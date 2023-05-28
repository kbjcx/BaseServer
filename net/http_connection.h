#ifndef BASESERVER_NET_HTTP_CONNECTION_H_
#define BASESERVER_NET_HTTP_CONNECTION_H_

#include "tcp_connection.h"

class HttpConnection : public TcpConnection {
public:
    HttpConnection(int fd);
    ~HttpConnection();
};
#endif //BASESERVER_NET_HTTP_CONNECTION_H_
