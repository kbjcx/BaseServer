#include "poll_poller.h"
#include <cassert>
#include "new.h"
#include "event.h"

static int POLL_TIMEOUT = 10000;

PollPoller* PollPoller::new_instance() {
    New<PollPoller>::allocate();
}

PollPoller::PollPoller() : poll_fd_list_(0), io_event_list_(0),
                           poll_fd_map_(0) {
    
}

bool PollPoller::add_io_event(IOEvent* io_event) {
    int fd = io_event->get_fd();
    if (fd < 0) {
        return false;
    }
    auto it = io_event_map_.find(fd);
    if (it == io_event_map_.end()) {
        struct pollfd poll_fd_set;
        poll_fd_set.fd = fd;
        poll_fd_set.events &= IOEvent::EVENT_NONE;
        poll_fd_set.revents &= IOEvent::EVENT_NONE;
        
        if (io_event->is_read_event()) {
            poll_fd_set.events |= POLLIN;
        }
        if (io_event->is_write_event()) {
            poll_fd_set.events |= POLLOUT;
        }
        if (io_event->is_error_event()) {
            poll_fd_set.events |= POLLERR;
        }
        
        poll_fd_list_.push_back(poll_fd_set);
        io_event_map_.insert(std::make_pair(fd, io_event));
        poll_fd_map_.insert(std::make_pair(fd, poll_fd_list_.size() - 1));
    }
    else {
        // 已经在监听该事件
        printf("该事件已在监听 \n");
    }
}

bool PollPoller::update_io_event(IOEvent* io_event) {
    int fd = io_event->get_fd();
    if (fd < 0) {
        return false;
    }
    
    auto it = io_event_map_.find(fd);
    if (it != io_event_map_.end()) {
        auto it_ = poll_fd_map_.find(fd);
        if (it_ == poll_fd_map_.end()) {
            // 没有加入索引查询哈希表
            return false;
        }
        struct pollfd& poll_fd_set = poll_fd_list_.at(it_->second);
        poll_fd_set.events &= IOEvent::EVENT_NONE;
        poll_fd_set.revents &= IOEvent::EVENT_NONE;
        
        if (io_event->is_read_event()) {
            poll_fd_set.events |= POLLIN;
        }
        if (io_event->is_write_event()) {
            poll_fd_set.events |= POLLOUT;
        }
        if (io_event->is_error_event()) {
            poll_fd_set.events |= POLLERR;
        }
    }
}

bool PollPoller::remove_io_event(IOEvent* io_event) {
    int fd = io_event->get_fd();
    if (fd < 0) {
        return false;
    }
    // 不存在该事件
    auto it = io_event_map_.find(fd);
    if (it == io_event_map_.end()) {
        return false;
    }
    auto it_ = poll_fd_map_.find(fd);
    if (it_ == poll_fd_map_.end()) {
        // 没有加入索引哈希表，但是存在于监听时间中
        return false;
    }
    int index = it_->second;
    if (index != poll_fd_list_.size()) {
        // 将要删除的元素交换到数组末尾
        std::iter_swap(poll_fd_list_.begin() + index, poll_fd_list_.end() - 1);
        // 将交换的元素的索引哈希表进行更新
        it_ = poll_fd_map_.find(poll_fd_list_.at(index).fd);
        it_->second = index;
    }
    // 删除列表最后的元素
    poll_fd_list_.pop_back();
    poll_fd_map_.erase(fd);
    io_event_map_.erase(fd);
    
    return true;
}

void PollPoller::handle_event() {
    if (poll_fd_list_.empty()) {
        return;
    }
    
    int nums = poll(&poll_fd_list_.front(), poll_fd_list_.size(), POLL_TIMEOUT);
    if (nums < 0) {
        // TODO
        return;
    }
    
    int events, revents = 0, fd;
    
    for (auto it = poll_fd_list_.begin(); it != poll_fd_list_.end() && nums > 0;
         ++it) {
        events = it->events;
        if (events > 0) {
            fd = it->fd;
            auto event_it = io_event_map_.find(fd);
            assert(event_it != io_event_map_.end());
            
            if (events & POLLIN || events & POLLPRI || events & POLLHUP) {
                revents |= IOEvent::EVENT_READ;
            }
            if (events & POLLOUT) {
                revents |= IOEvent::EVENT_WRITE;
            }
            if (events & POLLERR) {
                revents |= IOEvent::EVENT_ERROR;
            }
            event_it->second->set_return_event(revents);
            event_it->second->handle_event();
            --nums;
        }
        
    }
}

PollPoller::~PollPoller() = default;
