#ifndef BASESERVER_NET_ACCEPTOR_H_
#define BASESERVER_NET_ACCEPTOR_H_

class Acceptor {
public:
    using NewConnectionCallback = void (*)(void*, int);
    
    Acceptor();
    ~Acceptor();
    
    bool is_listening() const {
        return listening_;
    }
    
    int listen();
    void set_new_connection_callback(NewConnectionCallback cb);
    
    
private:
    
    bool listening_;
};

#endif //BASESERVER_NET_ACCEPTOR_H_
