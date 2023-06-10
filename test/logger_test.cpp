
#include "logger.h"
#include "async_logger.h"

int main() {
    Logger::set_log_level(Logger::LOG_WARNING);
//    Logger::output2stdout();
    Logger::output2file();
    for (int i = 0; i < 10000; i++) {
        EXPORT_ERROR_LOG("error, %d \n", i);
    }
    for (int i = 0; i < 10000; i++) {
        EXPORT_DEBUG_LOG("debug, %d \n", i);
    }
    for (int i = 0; i < 10000; i++) {
        EXPORT_WARNING_LOG("warning, %d \n", i);
    }
    
    while (1) {
    
    }
    
    return 0;
}
