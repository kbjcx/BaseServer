#include "allocator.h"
#include "mutex.h"
#include <cstdlib>

Allocator* Allocator::instance_ = nullptr;

Allocator::Allocator() : mutex_(new Mutex),
                         start_free_(nullptr),
                         end_free_(nullptr),
                         heap_size_(0),
                         free_list_() {
    
}

Allocator::~Allocator() = default;

void* Allocator::allocate(uint32_t size) {
    return instance()->alloc(size);
}

void Allocator::deallocator(void* ptr, uint32_t size) {
    instance()->dealloc(ptr, size);
}

Allocator* Allocator::instance() {
    if (!instance_) {
        instance_ = new Allocator();
    }
    return instance_;
}

void* Allocator::alloc(uint32_t size) {
    Obj* result;
    uint32_t index;
    
    // 如果要分配的内存大于MAX_BYTES，直接通过malloc分配
    if (size > MAX_BYTES) {
        void* ptr = malloc(size);
        if (ptr != nullptr) return ptr;
        else {
            // TODO 处理分配失败的问题
        }
    }
    
    MutexLockGuard lock_guard(mutex_);
    
    index = free_list_index(size);
    result = free_list_[index];
    
    // 如果没有找到则重新分配
    if (!result) {
        void* r = refill(roundup(size));
        return r;
    }
    
    // 找到了就从链表中删除该内存块
    free_list_[index] = result->next;
    
    return result;
}

void Allocator::dealloc(void* ptr, uint32_t size) {
    Obj* obj = (Obj*) ptr;
    uint32_t  index;
    
    if (size > MAX_BYTES) {
        free(ptr);
    }
    
    MutexLockGuard lock_guard(mutex_);
    index = free_list_index(size);
    
    // 将内存块添加到列表
    obj->next = free_list_[index];
    free_list_[index] = obj;
}

// bytes大小的内存链表为空，分配足够内存
void* Allocator::refill(uint32_t bytes) {
    int nobjs = 20;
    char* chunk = chunk_alloc(bytes, nobjs);
    Obj* result;
    Obj* current_obj;
    Obj* next_obj;
    
    int i;
    uint32_t index;
    
    // 如果只有一个节点，那么直接放回，不需要处理剩余内存
    if (nobjs == 1) {
        return chunk;
    }
    
    result = (Obj*) chunk;
    
    index = free_list_index(bytes);
    next_obj = (Obj*)(chunk + bytes);
    free_list_[index] = next_obj;
    
    // 将剩余内存连成链表
    for (i = 1; ;++i) {
        current_obj = next_obj;
        next_obj = (Obj*)((char*)next_obj + bytes);
        
        if (nobjs - 1 == i) {
            current_obj->next = nullptr;
            break;
        }
        else {
            current_obj->next = next_obj;
        }
    }
    
    return result;
}

char* Allocator::chunk_alloc(uint32_t size, int& nobjs) {
    char* result;
    uint32_t total_bytes = size * nobjs;
    uint32_t bytes_left = end_free_ - start_free_;
    
    if (bytes_left > total_bytes) {
        result = start_free_;
        start_free_ += total_bytes;
        return result;
    }
    else if (bytes_left > size) { // 空闲内存能分配多少就分配多少
        nobjs = bytes_left / size;
        total_bytes = size * nobjs;
        result = start_free_;
        start_free_ += total_bytes;
        return result;
    }
    else {
        uint32_t bytes2get = 2 * total_bytes + roundup(heap_size_ >> 4);
        
        if (bytes_left > 0) {
            uint32_t index = free_list_index(bytes_left);
            auto* obj = (Obj*)start_free_;
            obj->next = free_list_[index];
            free_list_[index] = obj;
        }
        
        // 重新申请缓存
        start_free_ = (char*) malloc(bytes2get);
        heap_size_ += bytes2get;
        end_free_ = start_free_ + bytes2get;
        
        // 递归调用，再次分配
        chunk_alloc(size, nobjs);
    }
}
