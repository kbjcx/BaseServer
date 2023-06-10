#ifndef BASESERVER_ALLOCATOR_CONSTRUCT_H_
#define BASESERVER_ALLOCATOR_CONSTRUCT_H_

#include <new>

template<class T>
inline void destroy(T* ptr) {
    ptr->~T();
}

template<class T>
inline void construct(T* ptr) {
    new (ptr) T();
}

template<class T, class T1>
inline void construct(T* ptr, const T1& t1) {
    new (ptr) T(t1);
}

template<class T, class T1, class T2>
inline void construct(T* ptr, const T1& t1, const T2& t2) {
    new (ptr) T(t1, t2);
}

template<class T, class T1, class T2, class T3>
inline void construct(T* ptr, const T1& t1, const T2& t2, const T3& t3) {
    new (ptr) T(t1, t2, t3);
}

template<class T, class T1, class T2, class T3, class T4>
inline void construct(T* ptr, const T1& t1, const T2& t2, const T3& t3,
                      const T4& t4) {
    new (ptr) T(t1, t2, t3, t4);
}

#endif
