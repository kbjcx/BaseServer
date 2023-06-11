#ifndef BASE_SERVER_BASE_EPOLL_POLLER_H_
#define BASE_SERVER_BASE_EPOLL_POLLER_H_

#include <vector>
#include <sys/epoll.h>
#include "poller.h"

class EpollPoller : public Poller {
public:
    static EpollPoller* new_instance();
    EpollPoller();
    ~EpollPoller() override;
    
    bool add_io_event(IOEvent *io_event) override;
    bool update_io_event(IOEvent *io_event) override;
    bool remove_io_event(IOEvent *io_event) override;
    
    void handle_event() override;
    
private:
    int epoll_fd_;
    using EpollEventList = std::vector<epoll_event>;
    EpollEventList epoll_event_list_;
    std::vector<IOEvent*> io_event_list_;
};

#endif
