# BaseServer
服务端的Base代码, 个人对于不同用途的服务端通用的代码部分

## 总体需求
接收Tcp连接, 获取协议请求并解析(如HTTP, RTSP等), 异步处理请求事务.
能够输出代码日志,方便定位错误.
实现内存池来节省内存开销.

## 大体结构
采取主从Reactor多线程模式
1. Main Reactor模块: 负责监听和分发事件, 连接事件分发到Acceptor, 其他事件分发到Sub Reactor
2. Acceptor模块: 与Main Reactor处于同一线程,负责处理连接事件, 并将新连接加入Sub Reactor进行监听
3. EventHandler模块: 负责解析由Sub Reactor分发的请求, 将事务交由线程池处理

## 重要组成
- []
- [线程池](./docs/ThreadPool.md)
- [网络连接模块](./docs/NetConnection.md)
- 内存池
- 日志模块
- ......
