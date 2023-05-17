#include "thread.h"

Thread::Thread() : thread_id_{}, is_start_(false), is_detach_(false),
                   arg_(nullptr) {
    
}

Thread::~Thread() {
    if (is_start_ && !is_detach_) {
        detach();
    }
}

bool Thread::start(void* arg) {
    arg_ = arg;
    if (pthread_create(&thread_id_, nullptr, thread_run, this) != 0) {
        return false;
    }
    is_start_ = true;
    return true;
}

bool Thread::detach() {
    if (!is_start_) {
        return false;
    }
    
    if (is_detach_) {
        return true;
    }
    
    if (pthread_detach(thread_id_) != 0) {
        return false;
    }
    
    is_detach_ = true;
    return true;
}

bool Thread::join() {
    if (!is_start_ || is_detach_) {
        return false;
    }
    
    if (pthread_join(thread_id_, nullptr) != 0) {
        return false;
    }
    
    return true;
}

bool Thread::cancel() {
    if (!is_start_) {
        return false;
    }
    
    if (pthread_cancel(thread_id_) != 0) {
        return false;
    }
    
    is_start_ = false;
    return true;
}

void* Thread::thread_run(void* arg) {
    auto thread = (Thread*) arg;
    thread->run(thread->arg_);
    return nullptr;
}
