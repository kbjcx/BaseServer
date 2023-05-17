#ifndef BASE_SERVER_BASE_THREAD_H_
#define BASE_SERVER_BASE_THREAD_H_

#include <pthread.h>

class Thread {
public:
    virtual ~Thread();
    bool start(void* arg);
    bool detach();
    bool join();
    bool cancel();
    pthread_t get_thread_id() const {
        return thread_id_;
    }
protected:
    Thread();
    
    virtual void run(void* arg) = 0;
private:
    static void* thread_run(void*);
private:
    pthread_t thread_id_;
    bool is_start_;
    bool is_detach_;
    void* arg_;
};

#endif
