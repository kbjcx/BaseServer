#ifndef BASESERVER_ALLOCATOR_ALLOCATOR_H_
#define BASESERVER_ALLOCATOR_ALLOCATOR_H_

#include <cstdint>

class Mutex;
class Allocator {
public:
    enum {ALIGN = 8};
    enum {MAX_BYTES = 128};
    enum {NFREELISTS = MAX_BYTES / ALIGN};
    
    union Obj {
        union Obj* next;
        char data[1];
    };
    
    static void* allocate(uint32_t size);
    static void deallocator(void* ptr, uint32_t size);
    
private:
    Allocator();
    
    ~Allocator();
    
    static Allocator* instance();
    
    void* alloc(uint32_t size);
    void dealloc(void* ptr, uint32_t size);
    
    // 获取指定字节数在自由链表的下标
    uint32_t free_list_index(uint32_t bytes) {
        // 向上取整
        return (bytes + ALIGN - 1) / ALIGN - 1;
    }
    
    // 字节对齐
    uint32_t roundup(uint32_t bytes) {
        return (bytes + ALIGN - 1) & (~(ALIGN - 1));
    }
    
    void* refill(uint32_t bytes);
    char* chunk_alloc(uint32_t size, int& nobjs);
    
private:
    static Allocator* instance_;
    
    Mutex* mutex_;
    
    // 维护的内存块
    char* start_free_;
    char* end_free_;
    uint32_t heap_size_;
    
    Obj* free_list_[NFREELISTS];
};

#endif
