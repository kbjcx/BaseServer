#ifndef BASESERVER_BASE_LOGGING_H_
#define BASESERVER_BASE_LOGGING_H_

#include <string>

#define debug(format, ...) \
    Logger::instance()->log(Logger::LOG_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

class Logger {
public:
    enum LogLevel {
        LOG_NONE = -2, // 所有级别都不打印
        LOG_ALL = -1, // 所有级别都打印
        LOG_PANIC = 0,
        LOG_ERROR,
        LOG_WARNING,
        LOG_INFO,
        LOG_DEBUG,
        LOG_COUNT // 代表有多少个级别的日志等级
    };
    static Logger* instance();
    
    static void log(LogLevel log_level, const char* file, int line,
                    const char* format, ...);
    // 重定向日志到文件
    // void redirect(std::FILE* file, LogLevel log_level = LogLevel::LOG_ALL);
    // 设置日志级别
    void set_level(LogLevel log_level);
    void open(const char* filename);
    void close();
    void max(int bytes);
    
private:
    Logger();
    ~Logger();
    void rotate();
    
private:
    static Logger* instance_;
    LogLevel log_level_;
    std::FILE* file_;
    std::string filename_;
    int max_;
    int len_;
    static const char* s_level[LOG_COUNT];
};

#endif
