
#include <thread_pool.h>
#include "async_logger.h"
#include "logger.h"

void task_func(void* arg) {
    for (int i = 0; i < 10; ++i) {
        EXPORT_DEBUG_LOG("%lu: hello, %d \n", pthread_self(), i);
    }
}

int main() {
    Logger::output2file();
    Logger::set_log_level(Logger::LOG_DEBUG);
    auto* thread_pool = new ThreadPool(8, 1000);
    for (int i = 0; i < 10000; ++i) {
        auto* task = new ThreadTask();
        task->set_task(task_func, nullptr);
        thread_pool->add_task(task);
    }
    
    while (1) {
    
    }
    
    return 0;
}
