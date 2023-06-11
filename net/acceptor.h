#ifndef BASESERVER_NET_ACCEPTOR_H_
#define BASESERVER_NET_ACCEPTOR_H_

class EventHandler;
class IOEvent;
/*
 * Acceptor设计为单例模式
 * 接收Server创建的socket来注册新连接事件
 * 事件当中注册处理新新连接的处理函数
 */
class Acceptor {
public:
    static Acceptor* instance();
    
    using NewConnectionCallback = void (*)(void*, int);
    
    ~Acceptor();
    
    bool is_listening() const {
        return listening_;
    }
    
    int listen();
    void set_new_connection_callback(NewConnectionCallback cb, void* arg);
    void set_fd(int fd);
    void set_main_reactor(EventHandler* main_reactor);
    
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
    IOEvent* accept_event_;
    EventHandler* main_reactor_;
};

#endif //BASESERVER_NET_ACCEPTOR_H_
