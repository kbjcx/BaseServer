#include "http_server.h"
#include "http_connection.h"

HttpServer::HttpServer(const Ipv4Address& ipv_4_address) :
        TcpServer(ipv_4_address) {
    
}

HttpServer::~HttpServer() noexcept {

}

void HttpServer::handle_new_connection(int fd) {
    // TODO 将新连接交给SubReactors处理
    auto connection = new HttpConnection(fd);
    connection->set_disconnection_callback(disconnection_callback, this);
    sub_reactors_->add_connection(connection);
}

void HttpServer::handle_disconnection(int fd) {
    // TODO 处理连接断开事件
    auto it = connections_map_.find(fd);
    if (it != connections_map_.end()) {
        sub_reactors_->handle_disconnection(it->second);
        connections_map_.erase(fd);
    }
}
