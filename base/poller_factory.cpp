#include "poller_factory.h"
#include "new.h"

PollerFactory* PollerFactory::instance_ = NEW(PollerFactory)();
PollerFactory::GC PollerFactory::gc_ = PollerFactory::GC();

PollerFactory::PollerFactory() = default;

PollerFactory::~PollerFactory() = default;

Poller* PollerFactory::create_poller(const std::string& mode) {
    if (mode == "poll") {
        return new PollPoller();
    }
    else if (mode == "epoll") {
        return new EpollPoller();
    }
    else if (mode == "select") {
        return nullptr;
    }
    else {
        // 错误
        return nullptr;
    }
}

PollerFactory* PollerFactory::get_instance() {
    return instance_;
}
