#include "async_logger.h"
#include <cassert>

LogBuffer::LogBuffer() : data_(), cur_ptr_(data_) {}

LogBuffer::~LogBuffer() = default;

void LogBuffer::append(const char* buffer, size_t len) {
    if (available() > len) {
        memcpy(cur_ptr_, buffer, len);
        cur_ptr_ += len;
    }
}

AsyncLogger* AsyncLogger::instance_ = nullptr;

AsyncLogger::AsyncLogger(std::string file)
        : file_(std::move(file)), run_(true), mutex_(new Mutex()),
        cond_(new Cond()), file_fd(nullptr), cur_buffer_(nullptr) {
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

AsyncLogger::~AsyncLogger() noexcept {
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
    
    // 释放所有阻塞线程
    cond_->broadcast();
    
    delete mutex_;
    delete cond_;
}

AsyncLogger* AsyncLogger::get_instance() {
    if (!instance_) {
    
    }
}
