#ifndef BASESERVER_NET_CONNECTION_HANDLER_H_
#define BASESERVER_NET_CONNECTION_HANDLER_H_

#include "ThreadPool.h"
#include "tcp_connection.h"
#include "tcp_server.h"

/*
 * SubReactor集合, 用于处理连接事件, 每个SubReactor工作于子线程, 创建一个简单的线程池
 * 用于处理请求读写事件
 */
class TcpServer;
class ConnectionHandler {
public:
    ConnectionHandler(TcpServer* server, int num);
    ~ConnectionHandler();
    
    static void disconnection_callback(void*);
    
    void add_connection(TcpConnection* tcp_connection);
    void handle_disconnection(TcpConnection* tcp_connection);
    
private:
    void handle_disconnection_list();
    
private:
    int index_;
    TcpServer* server_;
    ThreadPool* thread_pool_;
    std::vector<EventHandler*> sub_reactors_;
    std::vector<TcpConnection*> disconnection_list_;
    TriggerEvent* trigger_event_;
};

#endif
