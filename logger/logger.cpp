#include "logger.h"

#include <ctime>
#include <sys/time.h>
#include <cstring>
#include <cstdarg>
#include "async_logger.h"

Logger::LogLevel Logger::log_level_ = Logger::LOG_DEBUG;

bool Logger::is_stdout_ = true;

Logger::Logger() : data_(), cur_ptr_(data_), this_log_level_(LOG_DEBUG) {

}

Logger::~Logger() {
    if (is_stdout_) {
        printf("%s \n", data_);
    }
    else {
        AsyncLogger::instance()->append(data_, (int)(cur_ptr_ - data_));
    }
}

void Logger::set_log_level(Logger::LogLevel log_level) {
    log_level_ = log_level;
}

Logger::LogLevel Logger::get_log_level() {
    return log_level_;
}

void Logger::write(Logger::LogLevel log_level,
                   const char* file,
                   const char* function,
                   int line,
                   const char* format,
                   ...) {
    if (log_level > log_level_) {
        return;
    }
    
    timeval now{};
    gettimeofday(&now, nullptr);
    
    tm* system_time = localtime(&(now.tv_sec));
    
    this_log_level_ = log_level;
    
    sprintf(cur_ptr_, "%d-%02d-%02d %02d:%02d:%02d",
            system_time->tm_year + 1900,
            system_time->tm_mon + 1,
            system_time->tm_mday,
            system_time->tm_hour,
            system_time->tm_min,
            system_time->tm_sec);
    
    cur_ptr_ += strlen(cur_ptr_);
    
    if (log_level == LOG_DEBUG) {
        sprintf(cur_ptr_, " <DEBUG> ");
    }
    else if (log_level == LOG_WARNING) {
        sprintf(cur_ptr_, " <WARNING> ");
    }
    else if (log_level == LOG_ERROR) {
        sprintf(cur_ptr_, " <ERROR> ");
    }
    else {
        return;
    }
    
    cur_ptr_ += strlen(cur_ptr_);
    
    sprintf(cur_ptr_, "%s:%s:%d ", file, function, line);
    cur_ptr_ += strlen(cur_ptr_);
    
    va_list format_args;
    va_start(format_args, format);
    
    vsnprintf(cur_ptr_, sizeof(data_) - (cur_ptr_ - data_), format, format_args);
    
    va_end(format_args);
    cur_ptr_ += strlen(cur_ptr_);
}

void Logger::output2stdout() {
    is_stdout_ = true;
}

void Logger::output2file() {
    is_stdout_ = false;
}
