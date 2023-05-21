#ifndef BASE_SERVER_NET_TCP_SERVER_H_
#define BASE_SERVER_NET_TCP_SERVER_H_

#include "net_address.h"
#include "event.h"

class TcpServer {
public:
    virtual ~TcpServer() = default;
    
    int start();
    
    
protected:
    TcpServer(const Ipv4Address& ipv_4_address);
    virtual void handle_new_connection(int connection_fd) = 0;
    
protected:
    Ipv4Address ipv_4_address_;
    int server_fd_;
    
private:
    static void new_connection_callback(void* arg, int connection_fd);
    IOEvent* server_event_{};
    
    static void read_callback(void*);
    void handle_server_read();
};

#endif
