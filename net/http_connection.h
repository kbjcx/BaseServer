#ifndef BASESERVER_NET_HTTP_CONNECTION_H_
#define BASESERVER_NET_HTTP_CONNECTION_H_

#include "tcp_connection.h"

class HttpConnection : public TcpConnection {
public:
    explicit HttpConnection(int fd);
    ~HttpConnection();
    
    void handle_read() override;
    void handle_write() override;
    void handle_error() override;
};
#endif //BASESERVER_NET_HTTP_CONNECTION_H_
