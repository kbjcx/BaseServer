#ifndef BASESERVER_NET_ACCEPTOR_H_
#define BASESERVER_NET_ACCEPTOR_H_

#include "event.h"


/*
 * Acceptor设计为单例模式
 * 接收Server创建的socket来注册新连接事件
 * 事件当中注册处理新新连接的处理函数
 */
class Acceptor {
public:
    static Acceptor* get_instance();
    
    using NewConnectionCallback = void (*)(void*, int);
    
    ~Acceptor();
    
    bool is_listening() const {
        return listening_;
    }
    
    int listen();
    void set_new_connection_callback(NewConnectionCallback cb, void* arg);
    void set_fd(int fd);
    
    static void accept_callback(void*);
    void handle_accept();
private:
    Acceptor();
    
private:
    static Acceptor* instance_;
    NewConnectionCallback callback_;
    void* tcp_server_;
    bool listening_;
    int server_fd_;
    IOEvent* new_connection_event_;
};

#endif //BASESERVER_NET_ACCEPTOR_H_
