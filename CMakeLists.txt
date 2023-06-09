cmake_minimum_required(VERSION 3.16)
project(BaseServer)

set(CMAKE_CXX_STANDARD 14)

include_directories(base net utils logger)
link_libraries(pthread)

set(BASE base/thread_pool.cpp base/thread.cpp base/poller.cpp base/epoll_poller.cpp base/epoll_poller.h base/poll_poller.cpp base/poll_poller.h base/poller_factory.cpp base/poller_factory.h base/select_poller.cpp base/select_poller.h base/mutex.cpp base/mutex.h base/cond.cpp base/cond.h)
set(NET net/tcp_server.cpp base/event.cpp net/net_address.cpp net/acceptor.cpp net/acceptor.h net/tcp_connection.cpp net/tcp_connection.h base/event_handler.cpp base/event_handler.h net/http_server.cpp net/http_server.h net/connection_handler.cpp net/connection_handler.h net/http_connection.cpp net/http_connection.h)
set(UTIL utils/string_deal.h)
set(Logger logger/logger.cpp logger/async_logger.cpp logger/async_logger.h)

add_executable(BaseServer main.cpp ${BASE} ${NET} ${UTIL} ${Logger})
