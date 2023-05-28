#ifndef BASE_SERVER_NET_TCP_SERVER_H_
#define BASE_SERVER_NET_TCP_SERVER_H_

#include "net_address.h"
#include "event.h"
#include "acceptor.h"
#include "tcp_connection.h"
#include "connection_handler.h"

/*
 * TcpServer是服务的抽象类,其作用是启动服务
 * 根据不同使用需求,创建派生类来处理不同的连接与断开连接处理
 * 创建服务时将socket交给Accpetor处理新链接任务
 */
class TcpServer {
public:
     virtual ~TcpServer();
    
    void start();
    static void new_connection_callback(void*, int);
    static void disconnection_callback(void*, int);
    
    virtual void handle_new_connection(int fd) = 0;
    virtual void handle_disconnection(int fd) = 0;
    
protected:
    explicit TcpServer(const Ipv4Address& ipv_4_address);
    
protected:
    ConnectionHandler* sub_reactors_;
    std::unordered_map<int, TcpConnection*> connections_map_;
    EventHandler* main_reactor_;
    
private:
    bool is_start_;
    Ipv4Address ipv_4_address_;
    Acceptor* acceptor_;
};

#endif
