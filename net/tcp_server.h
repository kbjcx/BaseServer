#ifndef BASE_SERVER_NET_TCP_SERVER_H_
#define BASE_SERVER_NET_TCP_SERVER_H_

#include "net_address.h"

class TcpServer {
public:
    virtual ~TcpServer() = default;
    
    int start();
    int
    
protected:
    TcpServer(const Ipv4Address& ipv_4_address);
    virtual void handle_new_connection(int connection_fd) = 0;
    
protected:
    Ipv4Address ipv_4_address_;
    
private:
    static void new_connection_callback(void* arg, int connection_fd);
};

#endif
