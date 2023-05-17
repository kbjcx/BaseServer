#ifndef BASE_SERVER_BASE_THREAD_POOL_H_
#define BASE_SERVER_BASE_THREAD_POOL_H_

#include <pthread.h>
#include <list>
#include <vector>

#include "thread.h"

const int MAX_THREADS = 1024;
const int MAX_TASK_QUEUE = 65535;
/*!
 * @paragraph 任务结构体
 * @var function
 * @var argument
 */
struct ThreadPoolTask {
    void (*function)(void*);
    void* argument;
};


class ThreadPool {
public:
    enum ThreadPoolShutdownType {
        IMMEDIATE_SHUTDOWN = 1,
        GRACEFUL_SHUTDOWN = 2
    };
    
    enum {
        THREADPOOL_INVALID = -1,
        THREADPOOL_LOCK_FAILURE = -2,
        THREADPOOL_QUEUE_FULL = -3,
        THREADPOOL_SHUTDOWN = -4,
        THREADPOOL_THREAD_FAILURE = -5,
        THREADPOOL_GRACEFUL = 1
    };
    
    ThreadPool(int num_workers, int max_jobs);
    ~ThreadPool();
    
    void add_task(ThreadPoolTask* task);
private:
    class Thread_ : public Thread {
    protected:
        void run(void*) override;
    };
    
private:
    void create_threads();
    void cancel_threads();
    void handle_task();
private:
    std::vector<Thread_> thread_list_;
    std::list<ThreadPoolTask*> task_list_;
    int max_tasks_;
    int thread_sum_;
    int free_thread;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    bool quit_;
};

#endif
