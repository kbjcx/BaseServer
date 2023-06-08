#ifndef BASESERVER_BASE_EVENTHANDLER_H_
#define BASESERVER_BASE_EVENTHANDLER_H_

#include <vector>

#include "event.h"
#include "poller_factory.h"

/*
 * 用于处理各种事件
 * IO事件: 获取Poller中激活的IO事件进行处理, 管理IO事件的添加/删除/更新
 * 触发事件: 添加存储触发事件等待统一处理
 * 每个SubReactor都有一个EventHandler
 */
class TriggerEvent;
class EventHandler {
public:
    explicit EventHandler(std::string poller_mode);
    ~EventHandler();
    
    int add_io_event(IOEvent* event);
    int remove_io_event(IOEvent* event);
    int update_io_event(IOEvent* event);
    
    int add_trigger_event(TriggerEvent* event);
    
    void event_loop();
    
    void handle_trigger_events();
//    void handle_io_events();
    
private:
    bool quit_;
    Poller* poller_;
    PollerFactory* poller_factory_;
    std::vector<TriggerEvent*> trigger_event_list_;
};

#endif
