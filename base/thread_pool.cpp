#include <cstdio>
#include "thread_pool.h"
#include "new.h"

ThreadTask* ThreadTask::new_instance() {
    return New<ThreadTask>::allocate();
}

ThreadPool* ThreadPool::new_instance(int num_workers, int max_jobs) {
    return New<ThreadPool>::allocate(num_workers, max_jobs);
}

ThreadPool::ThreadPool(int num_workers, int max_tasks)
        : thread_list_(num_workers),
          max_tasks_(max_tasks),
          remaining_tasks_count_(0),
          thread_count_(num_workers),
          idle_thread_count_(num_workers),
          mutex_(),
          cond_(),
          quit_(false) {
    if (pthread_mutex_init(&mutex_, nullptr) != 0) {
//        throw ThreadPoolException(THREADPOOL_LOCK_FAILURE, __FILE__, __LINE__);
    }
    
    if (pthread_cond_init(&cond_, nullptr) != 0) {
//        throw ThreadPoolException(THREADPOOL_LOCK_FAILURE, __FILE__, __LINE__);
    }
    create_threads();
}

ThreadPool::~ThreadPool() {
    cancel_threads();
    
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&cond_);
}

void ThreadPool::add_task(ThreadTask* task) {
    pthread_mutex_lock(&mutex_);
    task_list_.push_back(task);
//    printf("task counts %zu \n", task_list_.size());
    pthread_cond_signal(&cond_);
    pthread_mutex_unlock(&mutex_);
}

void ThreadPool::handle_task() {
    while (!quit_) {
        ThreadTask* task;
        pthread_mutex_lock(&mutex_);
        if (task_list_.empty()) {
            pthread_cond_wait(&cond_, &mutex_);
        }
        
        if (quit_) {
            pthread_mutex_unlock(&mutex_);
            break;
        }
        
        if (task_list_.empty()) {
            pthread_mutex_unlock(&mutex_);
            continue;
        }
        
        task = task_list_.front();
        task_list_.pop_front();
        pthread_mutex_unlock(&mutex_);
        
        task->handle();
    }
}

void ThreadPool::create_threads() {
    if (pthread_mutex_lock(&mutex_) != 0) {
//        throw ThreadPoolException(THREADPOOL_LOCK_FAILURE, __FILE__, __LINE__);
    }
    for (auto& thread : thread_list_) {
        thread.start(this);
    }
    if (pthread_mutex_unlock(&mutex_) != 0) {
//        throw ThreadPoolException(THREADPOOL_LOCK_FAILURE, __FILE__, __LINE__);
    }
}

void ThreadPool::cancel_threads() {
    if (pthread_mutex_lock(&mutex_) != 0) {
//        throw ThreadPoolException(THREADPOOL_LOCK_FAILURE, __FILE__, __LINE__);
    }
    quit_ = true;
    if (pthread_cond_broadcast(&cond_) != 0) {
//        throw ThreadPoolException(THREADPOOL_LOCK_FAILURE, __FILE__, __LINE__);
    }
    for (auto& thread : thread_list_) {
        thread.join();
    }
    thread_list_.clear();
    if (pthread_mutex_unlock(&mutex_) != 0) {
//        throw ThreadPoolException(THREADPOOL_LOCK_FAILURE, __FILE__, __LINE__);
    }
}

void ThreadPool::Thread_::run(void* arg) {
    auto thread_pool = (ThreadPool*) arg;
    thread_pool->handle_task();
}
