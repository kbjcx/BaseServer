#ifndef BASESERVER_BASE_COND_H_
#define BASESERVER_BASE_COND_H_

#include <pthread.h>
class Mutex;
class Cond {
public:
    static Cond* new_instance();
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
