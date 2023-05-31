#include "logging.h"

Logger::LogLevel Logger::log_level_ = Logger::LogDebug;
std::string Logger::log_file_ = "/dev/stdout";
bool Logger::is_stdout_ = true;

Logger::Logger() : cur_ptr_(data_), this_log_level_(LogDebug), data_() {

}

Logger::~Logger() {
    if (is_stdout_) {
        printf("%s", data_);
    }
    else {
    
    }
}
