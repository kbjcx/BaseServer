#ifndef BASE_SERVER_NET_EVENT_H_
#define BASE_SERVER_NET_EVENT_H_

class Event {
protected:
    Event() = default;
    using EventCallback = void (*)(void*);
};

class IOEvent : public Event {
public:
    enum IOEventType {
        EVENT_NONE = 0,
        EVENT_READ = 1,
        EVENT_WRITE = 2,
        EVENT_ERROR = 4
    };
    
    IOEvent(int fd, void* arg);
    ~IOEvent() = default;
    
    int get_fd() const {
        return fd_;
    }
    
    int get_event() const {
        return event_;
    }
    
    void set_return_event(int event) {
        return_event_ = event;
    }
    
    void set_arg(void* arg) {
        arg_ = arg;
    }
    
    void set_read_callback(EventCallback cb) {
        read_callback_ = cb;
    }
    
    void set_write_callback(EventCallback cb) {
        write_callback_ = cb;
    }
    
    void set_error_callback(EventCallback cb) {
        error_callback_ = cb;
    }
    
    void enable_read() {
        event_ |= EVENT_READ;
    }
    
    void enable_write() {
        event_ |= EVENT_WRITE;
    }
    
    void enable_error() {
        event_ |= EVENT_ERROR;
    }
    
    void disable_read() {
        event_ &= (~EVENT_READ);
    }
    
    void disable_write() {
        event_ &= (~EVENT_WRITE);
    }
    
    void disable_error() {
        event_ &= (~EVENT_ERROR);
    }
    
    bool is_none_event() const {
        return event_ == EVENT_NONE;
    }
    
    bool is_read_event() const {
        return (event_ & EVENT_READ) > 0;
    }
    
    bool is_write_event() const {
        return (event_ & EVENT_WRITE) > 0;
    }
    
    bool is_error_event() const {
        return (event_ & EVENT_ERROR) > 0;
    }
    
    void handle_event();
    
private:
    int fd_;
    void* arg_;
    int event_;
    int return_event_;
    
    EventCallback read_callback_;
    EventCallback write_callback_;
    EventCallback error_callback_;
};

class TriggerEvent : public Event {
public:
    TriggerEvent() = default;
    ~TriggerEvent() = default;
    
    using DisconnectionCallback = void(*)(void*);
    
    void set_disconnection_callback(DisconnectionCallback cb, void* arg) {
        disconnection_callback_ = cb;
        arg_ = arg;
    }
    
private:
    DisconnectionCallback disconnection_callback_;
    void* arg_;
};

#endif
