# MainReactor

MainReactor的作用就是接收连接然后交给SubReactor处理. 主要包含三个功能: 1. 启动服务器 2. 监听连接事件 3. 将连接事件交给SubReactor.

## 类结构

### TcpServer

1. 启动服务
2. 抽象出处理连接事件和断连事件的函数

### Acceptor

1. 处理连接事件
2. 将新连接注册到SubReactor
3. 将服务端Socket注册到Poller

### TcpConnection

1. 抽象出连接类
2. 包含处理连接的基本函数: 处理断开连接事件, 处理读/写/错误函数

