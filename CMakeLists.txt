cmake_minimum_required(VERSION 3.16)
project(BaseServer)

set(CMAKE_CXX_STANDARD 14)

include_directories(base)
link_libraries(pthread)

set(BASE base/thread_pool.cpp base/thread.cpp)
set(UTIL utils/string_deal.h)

add_executable(BaseServer main.cpp ${BASE})
