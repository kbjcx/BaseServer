#ifndef BASESERVER_BASE_POLLER_FACTORY_H_
#define BASESERVER_BASE_POLLER_FACTORY_H_

#include <string>

class Poller;

class PollerFactory {
public:
    static PollerFactory* instance();
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
