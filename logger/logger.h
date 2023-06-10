#ifndef BASESERVER_LOGGER_LOGGER_H_
#define BASESERVER_LOGGER_LOGGER_H_

#include <string>

class Logger;
#define EXPORT_ERROR_LOG(format, ...) \
if (Logger::LOG_ERROR <= Logger::get_log_level()) \
    Logger().write(Logger::LOG_ERROR, __FILE__, __FUNCTION__, __LINE__, \
                   format, ##__VA_ARGS__)

#define EXPORT_DEBUG_LOG(format, ...) \
if (Logger::LOG_DEBUG <= Logger::get_log_level()) \
    Logger().write(Logger::LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, \
                   format, ##__VA_ARGS__)

#define EXPORT_WARNING_LOG(format, ...) \
if (Logger::LOG_WARNING <= Logger::get_log_level()) \
    Logger().write(Logger::LOG_WARNING, __FILE__, __FUNCTION__, __LINE__, \
                   format, ##__VA_ARGS__)

class Logger {
public:
    enum LogLevel {
        LOG_ERROR = 0,
        LOG_WARNING,
        LOG_DEBUG
    };
    
    Logger();
    ~Logger();
    
    static void set_log_level(LogLevel log_level);
    static LogLevel get_log_level();
    
    static void output2stdout();
    static void output2file();
    
    void write(LogLevel log_level, const char* file, const char* function,
               int line, const char* format, ...);
    
private:
    char data_[4096];
    char* cur_ptr_;
    LogLevel this_log_level_;
    
    static LogLevel log_level_;

    static bool is_stdout_;
};

#endif
