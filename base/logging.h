#ifndef BASESERVER_BASE_LOGGING_H_
#define BASESERVER_BASE_LOGGING_H_

#include <string>

class Logger {
public:
    enum LogLevel {
        LogError,
        LogWarning,
        LogDebug
    };
    
    Logger();
    ~Logger();
    
    static void set_log_file(std::string file);
    static std::string get_log_file();
    static void set_log_level(LogLevel level);
    static LogLevel get_log_level();
    
    void write(LogLevel level, const char* file, const char* func,
               int line, const char* format, ...);
    
private:
    char data_[4096];
    char* cur_ptr_;
    LogLevel this_log_level_;
    
    static LogLevel log_level_;
    static std::string log_file_;
    static bool is_stdout_;
};

#endif
