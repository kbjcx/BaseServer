# BaseServer
服务端的Base代码, 个人对于不同用途的服务端通用的代码部分

## 总体需求
接收Tcp连接, 获取协议请求并解析(如HTTP, RTSP等), 异步处理请求事务.
能够输出代码日志,方便定位错误.
实现内存池来节省内存开销.

## 大体结构
- 采取单Reactor多线程模式

包含Acceptor模块以及事务处理模块

## 重要组成
- [线程池](./docs/ThreadPool.md)
- [网络连接模块](./docs/NetConnection.md)
- 内存池
- 日志模块
- ......
