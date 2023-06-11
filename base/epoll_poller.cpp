#include "epoll_poller.h"
#include <unistd.h>
#include <cstring>
#include "new.h"
#include "event.h"

static const int init_event_list_size = 16;
static const int epoll_timeout = 10000;

EpollPoller* EpollPoller::new_instance() {
    New<EpollPoller>::allocate();
}

EpollPoller::EpollPoller() : epoll_event_list_(init_event_list_size),
                             epoll_fd_(-1){
    epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
    
    if (epoll_fd_ < 0) {
        printf("epoll fd create error \n");
    }
}

EpollPoller::~EpollPoller() {
    if (epoll_fd_ > 0) {
        ::close(epoll_fd_);
        epoll_fd_ = -1;
    }
}

bool EpollPoller::add_io_event(IOEvent* io_event) {
    int fd = io_event->get_fd();
    auto it = io_event_map_.find(fd);
    if (it != io_event_map_.end()) {
        return update_io_event(io_event);
    }
    
    struct epoll_event ev{};
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.fd = fd;
    if (io_event->is_read_event()) {
        ev.events |= EPOLLIN;
    }
    if (io_event->is_write_event()) {
        ev.events |= EPOLLOUT;
    }
    if (io_event->is_error_event()) {
        ev.events |= EPOLLERR;
    }
    epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
    io_event_map_.insert(std::make_pair(fd, io_event));
    if (io_event_map_.size() > epoll_event_list_.size()) {
        epoll_event_list_.resize(epoll_event_list_.size() * 2);
    }
    return true;
}

bool EpollPoller::update_io_event(IOEvent* io_event) {
    int fd = io_event->get_fd();
    epoll_event ev{};
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.fd = fd;
    if (io_event->is_read_event()) {
        ev.events |= EPOLLIN;
    }
    if (io_event->is_write_event()) {
        ev.events |= EPOLLOUT;
    }
    if (io_event->is_error_event()) {
        ev.events |= EPOLLERR;
    }
    epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev);
    return true;
}

bool EpollPoller::remove_io_event(IOEvent* io_event) {
    int fd = io_event->get_fd();
    auto it = io_event_map_.find(fd);
    if (it == io_event_map_.end()) {
        return false;
    }
    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
    io_event_map_.erase(fd);
    return true;
}

void EpollPoller::handle_event() {
    int nums, fd, event, r_event;
    nums = epoll_wait(epoll_fd_, &epoll_event_list_.front(),
                      epoll_event_list_.size(), epoll_timeout);
    if (nums < 0) {
        printf("epoll wait error \n");
        return;
    }
    
    IOEventMap::iterator it;
    for (int i = 0; i < nums; ++i) {
        r_event = 0;
        fd = epoll_event_list_.at(i).data.fd;
        event = epoll_event_list_.at(i).events;
        if (event & EPOLLIN || event & EPOLLPRI || event & EPOLLHUP) {
            r_event |= IOEvent::EVENT_READ;
        }
        if (event & EPOLLOUT) {
            r_event |= IOEvent::EVENT_WRITE;
        }
        if (event & EPOLLERR) {
            r_event |= IOEvent::EVENT_ERROR;
        }
        it = io_event_map_.find(fd);
        if (it == io_event_map_.end()) {
            printf("fd not in epoll event map \n");
            return;
        }
        it->second->set_return_event(r_event);
        io_event_list_.push_back(it->second);
    }
    
    for (auto& e : io_event_list_) {
        e->handle_event();
    }
    io_event_list_.clear();
}
