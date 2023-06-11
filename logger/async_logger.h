#ifndef BASESERVER_LOGGER_ASYNCLOGGER_H_
#define BASESERVER_LOGGER_ASYNCLOGGER_H_

#include <cstring>
#include <string>
#include <queue>
#include "thread.h"
#include "mutex.h"
#include "cond.h"

class LogBuffer {
public:
    static LogBuffer* new_instance();
    LogBuffer();
    ~LogBuffer();
    
    void append(const char* buffer, size_t len);
    
    const char* data() const {
        return data_;
    }
    
    int length() const {
        return (int)(cur_ptr_ - data_);
    }
    
    char* current() {
        return cur_ptr_;
    }
    
    int available() const {
        return (int)(end() - cur_ptr_);
    }
    
    void add(int len) {
        cur_ptr_ += len;
    }
    
    void reset() {
        cur_ptr_ = data_;
    }
    
    void bzero() {
        memset(data_, 0, BUFFER_SIZE);
    }
    
    
private:
    enum {
        BUFFER_SIZE = 1024*1024
    };
    
    const char* end() const {
        return data_ + BUFFER_SIZE;
    }
    
private:
    char data_[BUFFER_SIZE];
    char* cur_ptr_;
};

class AsyncLogger : public Thread {
public:
    ~AsyncLogger() override;
    
    static AsyncLogger* instance();
    
    void append(const char* log_line, int len);
    
protected:
    explicit AsyncLogger(std::string file);
    void run(void* arg) override;
    
private:
    enum {
        BUFFER_NUM = 4
    };
    
    static AsyncLogger* instance_;
    
    Mutex* mutex_;
    Cond* cond_;
    std::string file_;
    FILE* file_fd;
    bool run_;
    
    LogBuffer buffers_[BUFFER_NUM];
    LogBuffer* cur_buffer_;
    std::queue<LogBuffer*> free_buffers_;
    std::queue<LogBuffer*> flush_buffers_;
};


#endif
