#include <iostream>
#include <unistd.h>
#include "thread_pool.h"

void test(void* arg) {
    printf("--------------- \n");
    sleep(1);
}

int main() {
    auto task = new ThreadTask();
    task->set_task(test, nullptr);
    auto thread_pool = new thread_pool(3, 100);
    int i = 100;
    while (i > 0) {
        thread_pool->add_task(task);
        --i;
    }
    while (1) {
        printf("main \n");
        sleep(5);
    }

    return 0;
}
