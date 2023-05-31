#include "event_handler.h"

EventHandler::EventHandler() : poller_(nullptr), trigger_event_list_{},
                               quit_(false) {
    poller_ = new EpollPoller();
}

EventHandler::~EventHandler() {
    if (poller_ != nullptr) {
        delete poller_;
        poller_ = nullptr;
    }
}

int EventHandler::add_io_event(IOEvent* event) {
    poller_->add_io_event(event);
}

int EventHandler::update_io_event(IOEvent* event) {
    poller_->update_io_event(event);
}

int EventHandler::remove_io_event(IOEvent* event) {
    poller_->remove_io_event(event);
}

int EventHandler::add_trigger_event(TriggerEvent* event) {
    trigger_event_list_.push_back(event);
}

void EventHandler::event_loop() {
    while (!quit_) {
//        handle_trigger_events();
        poller_->handle_event();
    }
}

void EventHandler::handle_trigger_events() {
    for (auto& e : trigger_event_list_) {
        // TODO
    }
}
