#include "mutex.h"

Mutex::Mutex() : mutex_() {
    pthread_mutex_init(&mutex_, nullptr);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&mutex_);
}

void Mutex::lock() {
    pthread_mutex_lock(&mutex_);
}

void Mutex::unlock() {
    pthread_mutex_unlock(&mutex_);
}

pthread_mutex_t* Mutex::get() {
    return &mutex_;
}

MutexLockGuard::MutexLockGuard(Mutex* mutex_) : mutex_{mutex_} {
    mutex_->lock();
}

MutexLockGuard::~MutexLockGuard() {
    mutex_->unlock();
}
