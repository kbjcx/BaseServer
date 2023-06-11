#include "net_address.h"

Ipv4Address::Ipv4Address(std::string ip, uint16_t port)
        : ip_(std::move(ip)), port_(port), sockaddr_in_() {
    sockaddr_in_.sin_family = AF_INET;
    sockaddr_in_.sin_port = htons(port_);
    sockaddr_in_.sin_addr.s_addr = inet_addr(ip_.c_str());
}

void Ipv4Address::set_addr(std::string ip, uint16_t port) {
    ip_ = std::move(ip);
    port_ = port;
    sockaddr_in_.sin_family = AF_INET;
    sockaddr_in_.sin_port = htons(port_);
    sockaddr_in_.sin_addr.s_addr = inet_addr(ip_.c_str());
}
