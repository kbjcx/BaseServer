# 网络连接模块

## 需求分析
1. 接受连接,建立连接,注册到Epoller
2. 接收请求,解析请求,注册到Epoller
3. Socket处理

## 组成结构
1. TcpServer

    发起创建套接字, 绑定, 监听, 接收. 
2. TcpConnection

    建立连接的抽象类, 包含连接的公共处理函数
3. HttpConnection

    建立的Http连接类, 继承自TcpConnection, 能够完成Http请求的解析, 并将解析的任务注册到线程池的任务队列
4. RtspConnection

    建立的Rtsp连接类, 继承自TcpConnection, 能够完成Rtsp协议解析以及注册任务到线程池
