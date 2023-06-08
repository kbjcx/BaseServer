#ifndef BASESERVER_BASE_POLLER_FACTORY_H_
#define BASESERVER_BASE_POLLER_FACTORY_H_

#include "poller.h"
#include "epoll_poller.h"
#include "poll_poller.cpp"
#include <cstring>

class PollerFactory {
public:
    static PollerFactory* get_instance();
    ~PollerFactory();
    
    Poller* create_poller(const std::string& mode);

private:
    PollerFactory();
    class GC {
    public:
        ~GC () {
            if (instance_ != nullptr) {
                delete instance_;
                instance_ = nullptr;
            }
        }
    };
    
public:
    static PollerFactory* instance_;
    static GC gc_;
};

#endif
