#ifndef BASESERVER_BASE_POLLPOLLER_H_
#define BASESERVER_BASE_POLLPOLLER_H_

#include <vector>
#include <unordered_map>
#include <poll.h>
#include "poller.h"


class PollPoller : public Poller {
public:
    static PollPoller* new_instance();
    ~PollPoller() override;
    PollPoller();
    
    bool add_io_event(IOEvent *io_event) override;
    bool update_io_event(IOEvent *io_event) override;
    bool remove_io_event(IOEvent *io_event) override;
    
    void handle_event() override;
    
private:
    using PollFdList = std::vector<struct pollfd>;
    using PollFdMap = std::unordered_map<int, int>;
    PollFdList poll_fd_list_;
    PollFdMap poll_fd_map_;
    std::vector<IOEvent*> io_event_list_;
};

#endif
