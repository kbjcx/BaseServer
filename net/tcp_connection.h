#ifndef BASESERVER_NET_TCP_CONNECTION_H_
#define BASESERVER_NET_TCP_CONNECTION_H_

#include "event_handler.h"

class TcpConnection {
public:
    using disconnection_callback_t = void(*)(void*, int);
    void set_disconnection_callback(disconnection_callback_t disconnection_callback,
                                    void* arg) {
        disconnection_callback_ = disconnection_callback;
        tcp_server_ = arg;
    }
    
    void set_sub_reactor(EventHandler* sub_reactor) {
        sub_reactor_ = sub_reactor;
    }
    
    EventHandler* sub_reactor() const {
        return sub_reactor_;
    }
    
protected:
    TcpConnection(int fd);
    ~TcpConnection() = default;
    
private:
    EventHandler* sub_reactor_;
    disconnection_callback_t disconnection_callback_;
    void* tcp_server_;
};

#endif
