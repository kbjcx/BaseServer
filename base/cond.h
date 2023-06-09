#ifndef BASESERVER_BASE_COND_H_
#define BASESERVER_BASE_COND_H_

#include "mutex.h"
#include <pthread.h>

class Cond {
public:
    Cond();
    ~Cond();
    
    void wait(Mutex* mutex);
    bool wait_timeout(Mutex* mutex, int ms);
    void signal();
    void broadcast();
    
private:
    pthread_cond_t cond_;
};

#endif
