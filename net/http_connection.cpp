#include "http_connection.h"

HttpConnection::HttpConnection(int fd) : TcpConnection(fd) {}

HttpConnection::~HttpConnection() = default;

void HttpConnection::handle_read() {

}

void HttpConnection::handle_write() {

}

void HttpConnection::handle_error() {

}
