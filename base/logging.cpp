#include "logging.h"

//Logger::LogLevel Logger::log_level_ = Logger::LogDebug;
//std::string Logger::log_file_ = "/dev/stdout";
//bool Logger::is_stdout_ = true;

Logger::Logger() : file_(nullptr), log_level_(LOG_DEBUG) {

}

Logger::~Logger() = default;

void Logger::log(Logger::LogLevel log_level,
                 const char* file,
                 int line,
                 const char* format,
                 ...) {
    
}

void Logger::redirect(std::FILE* file, Logger::LogLevel log_level) {

}

void Logger::set_log_level(Logger::LogLevel log_level) {

}
