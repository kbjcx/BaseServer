#ifndef BASESERVER_BASE_EVENTHANDLER_H_
#define BASESERVER_BASE_EVENTHANDLER_H_

#include "event.h"

/*
 * 用于处理各种事件
 * IO事件: 获取Poller中激活的IO事件进行处理, 管理IO事件的添加/删除/更新
 * 触发事件: 添加存储触发事件等待统一处理
 * 每个SubReactor都有一个EventHandler
 */
class EventHandler {
public:
    EventHandler();
    ~EventHandler();
    
    int add_io_event(IOEvent* event);
    int remove_io_event(IOEvent* event);
    int update_io_event(IOEvent* event);
    
    int add_trigger_event(TriggerEvent* event);
    
    void event_loop();
    
};

#endif