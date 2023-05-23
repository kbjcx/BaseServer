#ifndef BASE_SERVER_NET_TCP_SERVER_H_
#define BASE_SERVER_NET_TCP_SERVER_H_

#include "net_address.h"
#include "event.h"
#include "acceptor.h"

/*
 * TcpServer是服务的抽象类,其作用是启动服务
 */
class TcpServer {
public:
    ~TcpServer();
    
    void start();
    
protected:
    TcpServer(Ipv4Address& ipv_4_address);
    
private:
    Acceptor* acceptor_;
};

#endif
