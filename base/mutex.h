#ifndef BASESERVER_BASE_MUTEX_H_
#define BASESERVER_BASE_MUTEX_H_

#include "pthread.h"

class Mutex {
public:
    static Mutex* new_instance();
    Mutex();
    ~Mutex();
    
    void lock();
    void unlock();
    
    pthread_mutex_t* get();
    
private:
    pthread_mutex_t mutex_;
};

class MutexLockGuard {
public:
    explicit MutexLockGuard(Mutex* mutex_);
    ~MutexLockGuard();
    
private:
    Mutex* mutex_;
};

#endif
