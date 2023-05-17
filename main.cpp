#include <iostream>
#include <unistd.h>
#include "thread_pool.h"

void test(void* arg) {
    printf("--------------- \n");
    sleep(5);
}

int main() {
    auto task = new ThreadPoolTask();
    
    auto thread_pool = new ThreadPool(10, 100);
    task->function = test;
    int i = 100;
    task->argument = nullptr;
    task->function(task->argument);
    while (i > 0) {
        thread_pool->add_task(task);
        --i;
    }
    while (1) {
        printf("main \n");
        sleep(5);
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
