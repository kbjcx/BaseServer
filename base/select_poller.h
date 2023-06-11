#ifndef BASESERVER_BASE_SELECTPOLLER_H_
#define BASESERVER_BASE_SELECTPOLLER_H_

#include <sys/select.h>
#include <vector>
#include "poller.h"

class SelectPoller : public Poller {
public:
    static SelectPoller* new_instance();
    SelectPoller();
    ~SelectPoller() override;
    
    bool add_io_event(IOEvent *io_event) override;
    bool update_io_event(IOEvent *io_event) override;
    bool remove_io_event(IOEvent *io_event) override;
    
    void handle_event() override;
    
private:
    fd_set read_set_;
    fd_set write_set_;
    fd_set exception_set_;
    int max_num_sockets_;
    std::vector<IOEvent*> io_event_list_;
};

#endif
