#include "tcp_server.h"

#include <sys/socket.h>
#include "acceptor.h"
#include "event_handler.h"
#include "connection_handler.h"


TcpServer::TcpServer(const Ipv4Address& ipv_4_address)
        : ipv_4_address_(ipv_4_address), acceptor_(nullptr),
          main_reactor_(nullptr), is_start_(false),
          sub_reactors_(nullptr) {
    
}

TcpServer::~TcpServer() {
    if (is_start_) {
        delete acceptor_;
        acceptor_ = nullptr;
        delete main_reactor_;
        main_reactor_ = nullptr;
        delete sub_reactors_;
        sub_reactors_ = nullptr;
    }
}

void TcpServer::start() {
    main_reactor_ = new EventHandler("epoll");
    // TODO 根据核心数设置Subreactor数量
    sub_reactors_ = new ConnectionHandler(this, 4);
    acceptor_ = Acceptor::instance();
    acceptor_->set_main_reactor(main_reactor_);
    // TODO 异常处理
    int server_fd = socket(PF_INET,
                           SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    int ret = bind(server_fd, ipv_4_address_.get_addr(), sizeof(sockaddr_in));
    
    acceptor_->set_fd(server_fd);
    acceptor_->set_new_connection_callback(new_connection_callback, this);
    acceptor_->listen();
}

void TcpServer::new_connection_callback(void* arg, int fd) {
    auto* tcp_server = (TcpServer*) arg;
    tcp_server->handle_new_connection(fd);
}

void TcpServer::disconnection_callback(void* arg, int fd) {
    auto* tcp_server = (TcpServer*) arg;
    tcp_server->handle_disconnection(fd);
}
