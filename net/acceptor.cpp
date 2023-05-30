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
                       listening_(false), server_fd_(-1), accept_event_(nullptr),
                       main_reactor_(nullptr) {
    accept_event_ = new IOEvent(server_fd_, this);
}

Acceptor::~Acceptor() {
    if (listening_) {
        delete accept_event_;
        // TODO 利用智能指针管理
    }
}

void Acceptor::set_fd(int fd) {
    server_fd_ = fd;
}

void Acceptor::set_main_reactor(EventHandler* main_reactor) {
    main_reactor_ = main_reactor;
}

void Acceptor::set_new_connection_callback(Acceptor::NewConnectionCallback cb,
                                           void* arg) {
    callback_ = cb;
    tcp_server_ = arg;
}

int Acceptor::listen() {
    printf("start listening \n");
    listening_ = true;
    ::listen(server_fd_, 1024);
    if (server_fd_ == -1) {
        // TODO 处理异常
    }
    accept_event_->enable_read();
    // 设置接收新连接的回调
    accept_event_->set_read_callback(accept_callback);
    main_reactor_->add_io_event(accept_event_);
    // TODO event_handler启动事件循环的顺序, 要在Subreactor创建之后
    main_reactor_->event_loop();
}

void Acceptor::accept_callback(void* arg) {
    printf("new connection \n");
    auto* acceptor = (Acceptor*) arg;
    acceptor->handle_accept();
}

void Acceptor::handle_accept() {
    printf("new connection \n");
    sockaddr_in client_addr{};
    memset(&client_addr, 0, sizeof(sockaddr_in));
    socklen_t len;
    int fd = ::accept(server_fd_, (sockaddr*)&client_addr, &len);
    // TODO 异常处理
    if (callback_ && tcp_server_) {
        callback_(tcp_server_, fd);
    }
}
