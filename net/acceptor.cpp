#include "acceptor.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include <cstring>

Acceptor* Acceptor::instance_ = nullptr;
Acceptor* Acceptor::get_instance() {
    if (instance_ == nullptr) {
        instance_ = new Acceptor();
    }
    return instance_;
}

Acceptor::Acceptor() : callback_(nullptr), tcp_server_(nullptr),
listening_(false), server_fd_(-1), new_connection_event_(nullptr),
new_connection_event_handler_(nullptr) {
    new_connection_event_ = new IOEvent(server_fd_, this);
    new_connection_event_handler_ = new EventHandler();
}

Acceptor::~Acceptor() {
    if (listening_) {
    
    }
}

void Acceptor::set_fd(int fd) {
    server_fd_ = fd;
}

void Acceptor::set_new_connection_callback(Acceptor::NewConnectionCallback cb,
                                           void* arg) {
    callback_ = cb;
    tcp_server_ = arg;
}

int Acceptor::listen() {
    listening_ = true;
    ::listen(server_fd_, 1024);
    if (server_fd_ == -1) {
        // TODO 处理异常
    }
    new_connection_event_->enable_read();
    // 设置接收新连接的回调
    new_connection_event_->set_read_callback(accept_callback);
    new_connection_event_handler_->add_io_event(new_connection_event_);
    // TODO event_handler还未启动事件循环
}

void Acceptor::accept_callback(void* arg) {
    auto* acceptor = (Acceptor*) arg;
    acceptor->handle_accept();
}

void Acceptor::handle_accept() {
    sockaddr_in client_addr{};
    memset(&client_addr, 0, sizeof(sockaddr_in));
    socklen_t len;
    int fd = ::accept(server_fd_, (sockaddr*)&client_addr, &len);
    // TODO 异常处理
    if (callback_ && tcp_server_) {
        callback_(tcp_server_, fd);
    }
}
