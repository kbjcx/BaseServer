#ifndef BASESERVER_NET_CONNECTION_HANDLER_H_
#define BASESERVER_NET_CONNECTION_HANDLER_H_

/*
 * SubReactor集合, 用于处理连接事件, 每个SubReactor工作于子线程, 创建一个简单的线程池
 * 用于处理请求读写事件
 */
class ConnectionHandler {
public:
    ConnectionHandler();
    ~ConnectionHandler();
    
private:

};

#endif
