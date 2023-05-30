#include <iostream>
#include <unistd.h>
#include "ThreadPool.h"
#include "epoll_poller.h"
#include "net_address.h"
#include "http_server.h"

int main() {
    Ipv4Address ipv_4_address("115.156.168.49", 9999);
    auto* server = new HttpServer(ipv_4_address);
    server->start();
    
    return 0;
}
