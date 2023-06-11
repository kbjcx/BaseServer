#ifndef BASESERVER_NET_HTTP_SERVER_H_
#define BASESERVER_NET_HTTP_SERVER_H_

#include "tcp_server.h"


class HttpServer : public TcpServer {
public:
    explicit HttpServer(const Ipv4Address& ipv_4_address);
    ~HttpServer() override;
    
    void handle_new_connection(int fd) override;
    void handle_disconnection(int fd) override;
    
private:

};

#endif
