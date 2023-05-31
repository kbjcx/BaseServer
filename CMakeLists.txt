cmake_minimum_required(VERSION 3.16)
project(BaseServer)

set(CMAKE_CXX_STANDARD 14)

include_directories(base net utils)
link_libraries(pthread)

set(BASE base/ThreadPool.cpp base/thread.cpp base/poller.cpp base/epoll_poller.cpp base/epoll_poller.h base/logging.cpp base/logging.h)
set(NET net/tcp_server.cpp base/event.cpp net/net_address.cpp net/acceptor.cpp net/acceptor.h net/tcp_connection.cpp net/tcp_connection.h base/event_handler.cpp base/event_handler.h net/http_server.cpp net/http_server.h net/connection_handler.cpp net/connection_handler.h net/http_connection.cpp net/http_connection.h)
set(UTIL utils/string_deal.h)

add_executable(BaseServer main.cpp ${BASE} ${NET} ${UTIL})
