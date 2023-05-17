#include <cstdio>
#include "thread_pool.h"

ThreadPool::ThreadPool(int num_workers, int max_tasks)
        : thread_list_(num_workers), max_tasks_(max_tasks),
        thread_sum_(num_workers), quit_(false), mutex_{}, cond_{},
        free_thread(num_workers) {
    pthread_mutex_init(&mutex_, nullptr);
    pthread_cond_init(&cond_, nullptr);
    create_threads();
}

ThreadPool::~ThreadPool() {
    cancel_threads();
    
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&cond_);
}

void ThreadPool::add_task(ThreadPoolTask* task) {
    pthread_mutex_lock(&mutex_);
    task_list_.push_back(task);
    pthread_cond_signal(&cond_);
    pthread_mutex_unlock(&mutex_);
}

void ThreadPool::handle_task() {
    while (!quit_) {
        ThreadPoolTask* task = nullptr;
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
        if (task != nullptr) {
            task->function(task->argument);
        }
    }
}

void ThreadPool::create_threads() {
    pthread_mutex_lock(&mutex_);
    for (auto& thread : thread_list_) {
        thread.start(this);
    }
    pthread_mutex_unlock(&mutex_);
}

void ThreadPool::cancel_threads() {
    pthread_mutex_lock(&mutex_);
    quit_ = true;
    pthread_cond_broadcast(&cond_);
    for (auto& thread : thread_list_) {
        thread.join();
    }
    thread_list_.clear();
    pthread_mutex_unlock(&mutex_);
}

void ThreadPool::Thread_::run(void* arg) {
    auto thread_pool = (ThreadPool*) arg;
    thread_pool->handle_task();
}
