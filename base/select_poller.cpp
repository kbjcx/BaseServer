#include "select_poller.h"
#include "new.h"
#include "event.h"


static const int SELECT_TIMEOUT = 10000;

SelectPoller* SelectPoller::new_instance() {
    return New<SelectPoller>::allocate();
}

SelectPoller::SelectPoller() : max_num_sockets_(0) {
    // 清空文件描述符集合
    FD_ZERO(&read_set_);
    FD_ZERO(&write_set_);
    FD_ZERO(&exception_set_);
}

SelectPoller::~SelectPoller() = default;

bool SelectPoller::add_io_event(IOEvent* io_event) {
    int fd = io_event->get_fd();
    if (fd < 0) {
        return false;
    }
    
    auto it = io_event_map_.find(fd);
    if (it != io_event_map_.end()) {
        if (io_event->is_read_event()) {
            FD_SET(fd, &read_set_);
        }
        if (io_event->is_write_event()) {
            FD_SET(fd, &write_set_);
        }
        if (io_event->is_error_event()) {
            FD_SET(fd, &exception_set_);
        }
        io_event_map_.insert(std::make_pair(fd, io_event));
        if (io_event_map_.empty()) {
            max_num_sockets_ = 0;
        }
        else {
            max_num_sockets_ = io_event_map_.rbegin()->first + 1;
        }
    }
    else {
        return false;
    }
}

bool SelectPoller::update_io_event(IOEvent* io_event) {
    int fd = io_event->get_event();
    if (fd < 0) {
        // TODO
        return false;
    }
    
    auto it = io_event_map_.find(fd);
    if (it != io_event_map_.end()) {
        FD_CLR(fd, &read_set_);
        FD_CLR(fd, &write_set_);
        FD_CLR(fd, &exception_set_);
        if (io_event->is_read_event()) {
            FD_SET(fd, &read_set_);
        }
        if (io_event->is_write_event()) {
            FD_SET(fd, &write_set_);
        }
        if (io_event->is_error_event()) {
            FD_SET(fd, &exception_set_);
        }
        
        if (io_event_map_.empty()) {
            max_num_sockets_ = 0;
        }
        else {
            max_num_sockets_ = io_event_map_.rbegin()->first + 1;
        }
    }
    else {
        // 不存在，请先加入
        return false;
    }
    return true;
}

bool SelectPoller::remove_io_event(IOEvent* io_event) {
    int fd = io_event->get_fd();
    if (fd < 0) {
        return false;
    }
    
    auto it = io_event_map_.find(fd);
    if (it != io_event_map_.end()) {
        FD_CLR(fd, &read_set_);
        FD_CLR(fd, &write_set_);
        FD_CLR(fd, &exception_set_);
        
        io_event_map_.erase(fd);
        if (io_event_map_.empty()) {
            max_num_sockets_ = 0;
        }
        else {
            max_num_sockets_ = io_event_map_.rbegin()->first + 1;
        }
        
        return true;
    }
    else {
        // 删除不存在的监听事件
        return false;
    }
}

void SelectPoller::handle_event() {
    fd_set read_set = read_set_;
    fd_set write_set = write_set_;
    fd_set exception_set = exception_set_;
    
    timeval timeout{};
    timeout.tv_sec = SELECT_TIMEOUT;
    timeout.tv_usec = 0;
    
    int ret = select(max_num_sockets_, &read_set, &write_set, &exception_set,
                     &timeout);
    if (ret < 0) {
        return;
    }
    
    int revents = IOEvent::EVENT_NONE;
    for (auto it = io_event_map_.begin(); it != io_event_map_.end(); ++it) {
        if (FD_ISSET(it->first, &read_set)) {
            revents |= IOEvent::EVENT_READ;
        }
        if (FD_ISSET(it->first, &write_set)) {
            revents |= IOEvent::EVENT_WRITE;
        }
        if (FD_ISSET(it->first, &exception_set)) {
            revents |= IOEvent::EVENT_ERROR;
        }
        
        if (revents != IOEvent::EVENT_NONE) {
            it->second->set_return_event(revents);
            it->second->handle_event();
        }
    }
}
