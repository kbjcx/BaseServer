#ifndef BASE_SERVER_BASE_EPOLL_POLLER_H_
#define BASE_SERVER_BASE_EPOLL_POLLER_H_

#include "poller.h"
#include <vector>
#include <sys/epoll.h>

class EpollPoller : public Poller {
public:
    static EpollPoller* get_instance();
    ~EpollPoller() override;
    
    bool add_io_event(IOEvent *io_event) override;
    bool update_io_event(IOEvent *io_event) override;
    bool remove_io_event(IOEvent *io_event) override;
    
    void handle_event() override;
    
protected:
    EpollPoller();
    
private:
    int epoll_fd_;
    using EpollEventList = std::vector<epoll_event>;
    EpollEventList epoll_event_list_;
    std::vector<IOEvent*> io_event_list_;
    static EpollPoller* instance_;
};

#endif
