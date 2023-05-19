#include "event.h"

IOEvent::IOEvent(int fd, void* arg) : fd_(fd),
                                      arg_(arg),
                                      event_(0),
                                      return_event_(0),
                                      read_callback_(nullptr),
                                      write_callback_(nullptr),
                                      error_callback_(nullptr) {
    
}

void IOEvent::handle_event() {
    if (is_none_event()) {
        return;
    }
    
    if (read_callback_ != nullptr && is_read_event()) {
        read_callback_(arg_);
    }
    
    if (write_callback_ != nullptr && is_write_event()) {
        write_callback_(arg_);
    }
    
    if (error_callback_ != nullptr && is_error_event()) {
        error_callback_(arg_);
    }
}
