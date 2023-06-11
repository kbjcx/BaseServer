#include "poller_factory.h"
#include "epoll_poller.h"
#include "poll_poller.h"
#include "select_poller.h"

PollerFactory* PollerFactory::instance_ = new PollerFactory();
PollerFactory::GC PollerFactory::gc_ = PollerFactory::GC();

PollerFactory::PollerFactory() = default;

PollerFactory::~PollerFactory() = default;

Poller* PollerFactory::create_poller(const std::string& mode) {
    if (mode == "poll") {
        return PollPoller::new_instance();
    }
    else if (mode == "epoll") {
        return EpollPoller::new_instance();
    }
    else if (mode == "select") {
        return SelectPoller::new_instance();
    }
    else {
        // 错误
        return nullptr;
    }
}

PollerFactory* PollerFactory::instance() {
    return instance_;
}
