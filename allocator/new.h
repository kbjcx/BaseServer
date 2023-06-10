#ifndef BASESERVER_ALLOCATOR_NEW_H_
#define BASESERVER_ALLOCATOR_NEW_H_

#include "allocator.h"
#include "construct.h"
template <class T>
class New;
class Delete;
#define NEW New::allocate
#define DELETE Delete::release

template <class T>
class New {
public:
    using Value = T;
    using Pointer = T*;
    using Ref = T&;

public:
    static Pointer allocate() {
        auto obj = (Pointer) Allocator::allocate(sizeof(Value));
        construct(obj);
        return obj;
    }
    
    template <class T1>
    static Pointer allocate(const T1& t1) {
        auto obj = (Pointer) Allocator::allocate(sizeof(Value));
        construct(obj, t1);
        return obj;
    }
    
    template <class T1, class T2>
    static Pointer allocate(const T1& t1, const T2& t2) {
        auto obj = (Pointer) Allocator::allocate(sizeof(Value));
        construct(obj, t1, t2);
        return obj;
    }
    
    template <class T1, class T2, class T3>
    static Pointer allocate(const T1& t1, const T2& t2,
                            const T3& t3) {
        auto obj = (Pointer) Allocator::allocate(sizeof(Value));
        construct(obj, t1, t2, t3);
        return obj;
    }
    
    template <class T1, class T2, class T3, class T4>
    static Pointer allocate(const T1& t1, const T2& t2,
                            const T3& t3, const T4& t4) {
        auto obj = (Pointer) Allocator::allocate(sizeof(Value));
        construct(obj, t1, t2, t3, t4);
        return obj;
    }
};

class Delete {
public:
    template <class T>
    static void release(T* pointer) {
        destroy(pointer);
        Allocator::deallocator(pointer, sizeof(T));
    }
};

#endif
