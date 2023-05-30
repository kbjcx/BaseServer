#ifndef BASE_SERVER_BASE_THREAD_POOL_H_
#define BASE_SERVER_BASE_THREAD_POOL_H_

#include <pthread.h>
#include <list>
#include <vector>

#include "thread.h"
#include "event_handler.h"

const int MAX_THREADS = 1024;
const int MAX_TASK_QUEUE = 65535;

class ThreadTask {
public:
    using task_func_t = void (*)(void*);
    using event_loop_t = void (EventHandler::*)();

public:
    ThreadTask() : task_callback_(nullptr), event_loop_callback_(nullptr),
                   arg_(nullptr) {
        
    }
    
    ~ThreadTask() = default;
    
    void set_task(task_func_t task_callback, void* arg) {
        task_callback_ = task_callback;
        arg_ = arg;
    }
    
    void set_task(event_loop_t event_loop_callback, void* arg) {
        event_loop_callback_ = event_loop_callback;
        arg_ = arg;
    }
    
    void handle() {
        if (task_callback_ != nullptr) {
            task_callback_(arg_);
        }
        if (event_loop_callback_ != nullptr) {
            ((EventHandler*) arg_->*event_loop_callback_)();
        }
    }

private:
    void* arg_;
    task_func_t task_callback_;
    event_loop_t event_loop_callback_;
};

class ThreadPool {
public:
    enum ShutdownType {
        IMMEDIATE_SHUTDOWN = 1,
        GRACEFUL_SHUTDOWN = 2
    };
    
    enum ErrorType {
        THREADPOOL_INVALID = -1,
        THREADPOOL_LOCK_FAILURE = -2,
        THREADPOOL_QUEUE_FULL = -3,
        THREADPOOL_SHUTDOWN = -4,
        THREADPOOL_THREAD_FAILURE = -5,
        THREADPOOL_GRACEFUL = 1
    };
    
    ThreadPool(int num_workers, int max_jobs);
    ~ThreadPool();
    
    void add_task(ThreadTask* task);
    
    int get_max_tasks_count() const {
        return max_tasks_;
    }
    
    int get_remaining_task_count() const {
        return remaining_tasks_count_;
    }
    
    int get_thread_count() const {
        return thread_count_;
    }
    
    int get_idle_thread_count() const {
        return idle_thread_count_;
    }

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
    std::list<ThreadTask*> task_list_;
    int max_tasks_;
    int remaining_tasks_count_;
    int thread_count_;
    int idle_thread_count_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    bool quit_;
};

class ThreadPoolException : public std::exception {
public:
    ThreadPoolException(ThreadPool::ErrorType error_type,
                        const char* filename,
                        int line_num)
            : error_type_(error_type),
              line_num_(line_num),
              filename_(filename),
              message_() {
        switch (error_type_) {
            case ThreadPool::THREADPOOL_LOCK_FAILURE:
                write_message("thread pool mutex or cond error");
                break;
            case ThreadPool::THREADPOOL_INVALID:
                write_message("thread pool is invalid");
                break;
            case ThreadPool::THREADPOOL_SHUTDOWN:
                write_message("thread pool is shutdown");
                break;
            case ThreadPool::THREADPOOL_GRACEFUL:
                write_message("thread pool is graceful shutdown");
                break;
            case ThreadPool::THREADPOOL_THREAD_FAILURE:
                write_message("thread fail");
                break;
            case ThreadPool::THREADPOOL_QUEUE_FULL:
                write_message("task queue is full");
                break;
            default:write_message("undefined error in thread pool");
        }
    }
    
    const char* what() const noexcept override {
        return message_;
    }

private:
    void write_message(const char* msg) {
        sprintf(message_, "%s filename: %s line number: %d \n",
                msg, filename_, line_num_);
    }

private:
    ThreadPool::ErrorType error_type_;
    const char* filename_;
    int line_num_;
    char message_[1024];
};

#endif
