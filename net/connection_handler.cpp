#include "connection_handler.h"

ConnectionHandler::ConnectionHandler(TcpServer* server, int num) :
        thread_pool_(nullptr), sub_reactors_(num), disconnection_list_(),
        trigger_event_(nullptr), server_(server) {
    thread_pool_ = new ThreadPool(num, 1024);
    for (int i = 0; i < num; ++i) {
        sub_reactors_[i] = new EventHandler();
        auto thread_task = new ThreadTask();
        thread_task->set_task(&EventHandler::event_loop, sub_reactors_[i]);
        thread_pool_->add_task(thread_task);
    }
}

ConnectionHandler::~ConnectionHandler() {
    delete thread_pool_;
    for (auto& sub_reactor : sub_reactors_) {
        delete sub_reactor;
    }
}

void ConnectionHandler::add_connection(TcpConnection* tcp_connection) {
    // TODO 按照一定的策略分配给subreactor,并将subreactor注册到connection中
    
}

void ConnectionHandler::handle_disconnection(TcpConnection* tcp_connection) {
    trigger_event_ = new TriggerEvent();
    trigger_event_->set_disconnection_callback(disconnection_callback, this);
    disconnection_list_.push_back(tcp_connection);
    server_->main_reactor_->add_trigger_event(trigger_event_);
}

void ConnectionHandler::disconnection_callback(void* arg) {
    auto* connection_handler = (ConnectionHandler*) arg;
    connection_handler->handle_disconnection_list();
}

void ConnectionHandler::handle_disconnection_list() {
    // TODO 处理断连列表
}
