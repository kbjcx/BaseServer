#include "http_server.h"

HttpServer::HttpServer(const Ipv4Address& ipv_4_address) :
        TcpServer(ipv_4_address) {
    
}

HttpServer::~HttpServer() noexcept {

}

void HttpServer::handle_new_connection(int fd) {
    // TODO 将新连接交给SubReactors处理
}

void HttpServer::handle_disconnection() {
    // TODO 处理连接断开事件
}
