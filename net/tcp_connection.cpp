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
