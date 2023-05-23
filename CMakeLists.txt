cmake_minimum_required(VERSION 3.16)
project(BaseServer)

set(CMAKE_CXX_STANDARD 14)

include_directories(base net utils)
link_libraries(pthread)

set(BASE base/thread_pool.cpp base/thread.cpp base/poller.cpp base/epoll_poller.cpp base/epoll_poller.h)
set(NET net/tcp_server.cpp base/event.cpp net/net_address.cpp net/acceptor.cpp net/acceptor.h net/TcpConnection.cpp net/TcpConnection.h net/BaseServer.cpp net/BaseServer.h base/event_handler.cpp base/event_handler.h)
set(UTIL utils/string_deal.h)

add_executable(BaseServer main.cpp ${BASE} ${NET} ${UTIL})
