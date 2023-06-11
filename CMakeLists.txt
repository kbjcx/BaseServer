cmake_minimum_required(VERSION 3.16)
project(BaseServer)

set(CMAKE_CXX_STANDARD 14)

include_directories(base net utils logger allocator)
link_libraries(pthread)

set(BASE base/thread_pool.cpp base/thread.cpp base/poller.cpp base/epoll_poller.cpp base/poll_poller.cpp base/poller_factory.cpp base/select_poller.cpp base/mutex.cpp base/cond.cpp base/singleton.h)
set(NET net/tcp_server.cpp base/event.cpp net/net_address.cpp net/acceptor.cpp net/tcp_connection.cpp base/event_handler.cpp net/http_server.cpp net/connection_handler.cpp net/http_connection.cpp )
set(UTIL )
set(Logger logger/logger.cpp logger/async_logger.cpp )
set(Allocator allocator/allocator.cpp allocator/new.cpp allocator/new.h)

add_executable(BaseServer main.cpp ${BASE} ${NET} ${UTIL} ${Logger} ${Allocator})

add_executable(LoggerTest test/logger_test.cpp base/thread.cpp base/cond.cpp base/mutex.cpp ${Logger} ${Allocator})

add_executable(ThreadPoolTest test/threadpool_test.cpp base/thread_pool.cpp base/thread.cpp base/cond.cpp base/mutex.cpp ${Logger} ${Allocator})
