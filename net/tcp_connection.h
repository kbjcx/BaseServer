#ifndef BASESERVER_NET_TCP_CONNECTION_H_
#define BASESERVER_NET_TCP_CONNECTION_H_

#include "event_handler.h"

/*
 * 处理连接的读写,将回调注册到IOevent中后将IOEvent注册到subreactor
 */
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
        sub_reactor_->add_io_event(tcp_connection_io_event_);
    }
    
    EventHandler* sub_reactor() const {
        return sub_reactor_;
    }
    
    void enable_read();
    void enable_write();
    void enable_error();
    void disable_read();
    void disable_write();
    void disable_error();
    
    virtual void handle_read() = 0;
    virtual void handle_write() = 0;
    virtual void handle_error() = 0;
    
private:
    static void read_callback(void* arg);
    static void write_callback(void* arg);
    static void error_callback(void* arg);
    
protected:
    TcpConnection(int fd);
    ~TcpConnection() = default;
    
private:
    EventHandler* sub_reactor_;
    IOEvent* tcp_connection_io_event_;
    disconnection_callback_t disconnection_callback_;
    void* tcp_server_;
};

#endif
