#ifndef MY_ALLOCATOR_TPP
#define MY_ALLOCATOR_TPP

#include <iostream>
#include <new>
#include <typeinfo>

#include "my_allocator.h"

template <typename T, std::size_t Capacity>
MyAllocator<T, Capacity>::MyAllocator() noexcept
    : m_pool(static_cast<T*>(::operator new(Capacity * sizeof(T)))), m_used(0) {
    std::cout << "default ctor: " << typeid(T).name() << "  " << "pool:" << m_pool << " - "
              << (m_pool + Capacity) << '\n';
}

// Конструктор копирования, у каждого аллокатора будет свой пул
template <typename T, std::size_t Capacity>
MyAllocator<T, Capacity>::MyAllocator(const MyAllocator&) noexcept
    : m_pool(static_cast<T*>(::operator new(Capacity * sizeof(T)))), m_used(0) {
    std::cout << "copy ctor same T: " << typeid(T).name() << "  " << "pool:" << m_pool << " - "
              << (m_pool + Capacity) << '\n';
}

template <typename T, std::size_t Capacity>
template <typename U>
MyAllocator<T, Capacity>::MyAllocator(const MyAllocator<U, Capacity>&) noexcept
    : m_pool(static_cast<T*>(::operator new(Capacity * sizeof(T)))), m_used(0) {
    std::cout << "copy ctor from U to T: " << typeid(U).name() << " -> " << typeid(T).name() << "  "
              << "pool:" << m_pool << " - " << (m_pool + Capacity) << '\n';
}

template <typename T, std::size_t Capacity>
MyAllocator<T, Capacity>::~MyAllocator() noexcept {
    ::operator delete(m_pool);
    std::cout << "dtor: " << typeid(T).name() << "  " << "pool:" << m_pool << " - "
              << (m_pool + Capacity) << '\n';
}

template <typename T, std::size_t Capacity>
T* MyAllocator<T, Capacity>::allocate(std::size_t n) {
    if (n == 0) {
        return nullptr;
    }

    if (m_used + n > Capacity) {
        throw std::bad_alloc();
    }
    T* result = m_pool + m_used;
    m_used += n;
    std::cout << "allocate: " << typeid(T).name() << " n=" << n << " used=" << m_used << " from "
              << static_cast<void*>(result) << '\n';
    return result;
}

template <typename T, std::size_t Capacity>
void MyAllocator<T, Capacity>::deallocate(T*, std::size_t) noexcept {
    // По условию задания поэлементное освобождение не требуется.
}

template <typename T, std::size_t Capacity, typename U, std::size_t OtherCapacity>
bool operator==(const MyAllocator<T, Capacity>&, const MyAllocator<U, OtherCapacity>&) noexcept {
    return Capacity == OtherCapacity;
}

template <typename T, std::size_t Capacity, typename U, std::size_t OtherCapacity>
bool operator!=(const MyAllocator<T, Capacity>& lhs,
                const MyAllocator<U, OtherCapacity>& rhs) noexcept {
    return !(lhs == rhs);
}

#endif  // MY_ALLOCATOR_TPP
