#include "cond.h"
#include <ctime>
#include <sys/time.h>
#include "mutex.h"
#include "new.h"

Cond* Cond::new_instance() {
    return New<Cond>::allocate();
}

Cond::Cond() : cond_() {
    pthread_cond_init(&cond_, nullptr);
}

Cond::~Cond() {
    pthread_cond_destroy(&cond_);
}

void Cond::wait(Mutex* mutex) {
    pthread_cond_wait(&cond_, mutex->get());
}

bool Cond::wait_timeout(Mutex* mutex, int ms) {
    timespec abs_time{};
    timespec now{};
    
    clock_gettime(CLOCK_REALTIME, &now);
    abs_time.tv_sec = now.tv_sec + ms / 1000;
    abs_time.tv_nsec = now.tv_nsec + ms % 1000 * 1000 * 1000;
    
    if (pthread_cond_timedwait(&cond_, mutex->get(), &abs_time) == 0) {
        return true;
    }
    else {
        return false;
    }
}

void Cond::signal() {
    pthread_cond_signal(&cond_);
}

void Cond::broadcast() {
    pthread_cond_broadcast(&cond_);
}
