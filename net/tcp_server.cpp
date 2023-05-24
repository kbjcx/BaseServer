#include "tcp_server.h"

#include <sys/socket.h>
#include "epoll_poller.h"
#include <cstring>

TcpServer::TcpServer(const Ipv4Address& ipv_4_address)
        : ipv_4_address_(ipv_4_address), acceptor_(nullptr) {
    acceptor_ = Acceptor::get_instance();
}

TcpServer::~TcpServer() {
    if (acceptor_ != nullptr) {
        delete acceptor_;
        acceptor_ = nullptr;
    }
}

void TcpServer::start() {
    // TODO 异常处理
    int server_fd = socket(PF_INET,
                           SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    int ret = bind(server_fd, ipv_4_address_.get_addr(), sizeof(sockaddr_in));
    acceptor_->set_fd(server_fd);
    acceptor_->set_new_connection_callback(new_connection_callback, this);
}

void TcpServer::new_connection_callback(void* arg, int fd) {
    auto* tcp_server = (TcpServer*) arg;
    tcp_server->handle_new_connection(fd);
}
