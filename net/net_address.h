#ifndef BASE_SERVER_NET_NET_ADDRESS_H_
#define BASE_SERVER_NET_NET_ADDRESS_H_

#include <cstdint>
#include <arpa/inet.h>
#include <string>

class Ipv4Address {
public:
    Ipv4Address(std::string ip, uint16_t port);
    ~Ipv4Address() = default;
    
    void set_addr(std::string ip, uint16_t port);
    
    std::string get_ip() const {
        return ip_;
    }
    
    uint16_t get_port() const {
        return port_;
    }
    
    sockaddr* get_addr() const {
        return (sockaddr*) &sockaddr_in_;
    }
    
private:
    std::string ip_;
    uint16_t port_;
    sockaddr_in sockaddr_in_;
};

// TODO Ipv6地址类实现
class Ipv6Address {
public:
    Ipv6Address() = default;
    ~Ipv6Address() = default;
};

#endif
