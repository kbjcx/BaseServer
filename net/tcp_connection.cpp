#include "tcp_connection.h"
#include "event.h"

TcpConnection::TcpConnection(int fd) : sub_reactor_(nullptr),
                                       disconnection_callback_(nullptr),
                                       tcp_server_(nullptr),
                                       tcp_connection_io_event_(nullptr) {
    tcp_connection_io_event_ = new IOEvent(fd, this);
    tcp_connection_io_event_->set_read_callback(read_callback);
    tcp_connection_io_event_->set_write_callback(write_callback);
    tcp_connection_io_event_->set_error_callback(error_callback);
    tcp_connection_io_event_->enable_read();
}

void TcpConnection::enable_read() {
    tcp_connection_io_event_->enable_read();
    sub_reactor_->update_io_event(tcp_connection_io_event_);
}

void TcpConnection::enable_write() {
    tcp_connection_io_event_->enable_write();
    sub_reactor_->update_io_event(tcp_connection_io_event_);
}

void TcpConnection::enable_error() {
    tcp_connection_io_event_->enable_error();
    sub_reactor_->update_io_event(tcp_connection_io_event_);
}

void TcpConnection::disable_read() {
    tcp_connection_io_event_->disable_read();
    sub_reactor_->update_io_event(tcp_connection_io_event_);
}

void TcpConnection::disable_write() {
    tcp_connection_io_event_->disable_write();
    sub_reactor_->update_io_event(tcp_connection_io_event_);
}

void TcpConnection::disable_error() {
    tcp_connection_io_event_->disable_error();
    sub_reactor_->update_io_event(tcp_connection_io_event_);
}

void TcpConnection::read_callback(void* arg) {
    auto* tcp_connection = (TcpConnection*) arg;
    tcp_connection->handle_read();
}

void TcpConnection::write_callback(void* arg) {
    auto* tcp_connection = (TcpConnection*) arg;
    tcp_connection->handle_write();
}

void TcpConnection::error_callback(void* arg) {
    auto* tcp_connection = (TcpConnection*) arg;
    tcp_connection->handle_error();
}
