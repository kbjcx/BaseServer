#include "async_logger.h"
#include "new.h"
#include <cassert>
AsyncLogger* AsyncLogger::instance_ = nullptr;
AsyncLogger* AsyncLogger::instance() {
    if (!instance_) {
        instance_ = new AsyncLogger("./server.log");
    }
    return instance_;
}

LogBuffer* LogBuffer::new_instance() {
    return New<LogBuffer>::allocate();
}

LogBuffer::LogBuffer() : data_(), cur_ptr_(data_) {}

LogBuffer::~LogBuffer() = default;

void LogBuffer::append(const char* buffer, size_t len) {
    if (available() > len) {
        memcpy(cur_ptr_, buffer, len);
        cur_ptr_ += len;
    }
}

AsyncLogger::AsyncLogger(std::string file)
        : file_(std::move(file)), run_(true), mutex_(Mutex::new_instance()),
        cond_(Cond::new_instance()), file_fd(nullptr), cur_buffer_(nullptr) {
    assert(mutex_);
    assert(cond_);
    
    file_fd = fopen(file_.c_str(), "w");
    assert(file_fd);
    
    for (auto& buffer : buffers_) {
        free_buffers_.push(&buffer);
    }
    
    cur_buffer_ = free_buffers_.front();
    
    start(nullptr);
}

AsyncLogger::~AsyncLogger() {
    while (!flush_buffers_.empty()) {
        LogBuffer* buffer = flush_buffers_.front();
        fwrite(buffer->data(), 1, buffer->length(), file_fd);
        flush_buffers_.pop();
    }
    
    // 将当前buffer存在部分写入文件
    fwrite(cur_buffer_->data(), 1, cur_buffer_->length(), file_fd);
    
    fflush(file_fd);
    fclose(file_fd);
    
    run_ = false;
    
    // 释放所有等待写的日志写入
    cond_->broadcast();
    
    delete mutex_;
    delete cond_;
}

void AsyncLogger::append(const char* log_line, int len) {
    MutexLockGuard lock_guard(mutex_);
    if (cur_buffer_->available() > len) {
        cur_buffer_->append(log_line, len);
    }
    else {
        free_buffers_.pop();
        flush_buffers_.push(cur_buffer_);
        
        while (free_buffers_.empty()) {
            // 通知写出线程将已有buffer写入文件
            cond_->signal();
            cond_->wait(mutex_);
        }
        
        cur_buffer_ = free_buffers_.front();
        cur_buffer_->append(log_line, len);
        // 通知其他线程可写
        cond_->signal();
    }
}

void AsyncLogger::run(void* arg) {
    while (run_) {
        MutexLockGuard lock_guard(mutex_);
        bool ret = cond_->wait_timeout(mutex_, 3000);
        if (!run_) break;
        
        if (ret) {
            bool empty = free_buffers_.empty();
            
            while (!flush_buffers_.empty()) {
                LogBuffer* buffer = flush_buffers_.front();
                fwrite(buffer->data(), 1, buffer->length(), file_fd);
                flush_buffers_.pop();
                buffer->reset();
                free_buffers_.push(buffer);
                fflush(file_fd);
            }
            
            // 当free为空时说明写入日志在wait，需要唤醒
            if (empty) cond_->signal();
        }
        else {
            if (cur_buffer_->length()  == 0) {
                continue;
            }
            fwrite(cur_buffer_->data(), 1, cur_buffer_->length(), file_fd);
            cur_buffer_->reset();
            fflush(file_fd);
        }
    }
}
