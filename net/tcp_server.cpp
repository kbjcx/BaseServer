#include "tcp_server.h"

#include <sys/socket.h>
#include "epoll_poller.h"
#include <cstring>

TcpServer::TcpServer(const Ipv4Address& ipv_4_address)
        : ipv_4_address_(ipv_4_address), server_event_(nullptr),
        server_fd_(-1) {
    
}

int TcpServer::start() {
    server_fd_ = ::socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                             0);
    if (server_fd_ < 0) {
        printf("socket create error \n");
        return server_fd_;
    }
    // TODO 异常处理
    int ret = ::bind(server_fd_, ipv_4_address_.get_addr(), sizeof(sockaddr_in));
    // flags可变参数
    ret = listen(server_fd_, 1024);
    server_event_ = new IOEvent(server_fd_, this);
    server_event_->set_read_callback(read_callback);
    server_event_->enable_read();
    EpollPoller::get_instance()->add_io_event(server_event_);
}

void TcpServer::read_callback(void* arg) {
    auto* tcp_server = (TcpServer*) arg;
    tcp_server->handle_server_read();
}

void TcpServer::handle_server_read() {
    sockaddr_in addr{};
    memset(&addr, 0, sizeof(sockaddr_in));
    socklen_t socklen = sizeof(sockaddr_in);
    int conn_fd = ::accept(server_fd_, (sockaddr*)&addr, &socklen);
//    sockets::set_non_block_and_close_on_exec(client_fd);
//    sockets::set_no_sigpipe(client_fd);
    auto* connection_event = new IOEvent(conn_fd, );
}
