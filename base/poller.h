#ifndef BASE_SERVER_BASE_POLLER_H_
    #define BASE_SERVER_BASE_POLLER_H_

#include "event.h"
#include <unordered_map>

class Poller {
public:
    virtual ~Poller() = default;
    
    virtual bool add_io_event(IOEvent* io_event) = 0;
    virtual bool update_io_event(IOEvent* io_event) = 0;
    virtual bool remove_io_event(IOEvent* io_event) = 0;
    
    virtual void handle_event() = 0;
    
protected:
    Poller() = default;
    
protected:
    using IOEventMap = std::unordered_map<int, IOEvent*>;
    IOEventMap io_event_map_;
};

#endif
