#ifndef MY_ALLOCATOR_TPP
#define MY_ALLOCATOR_TPP

#include <iostream>
#include <typeinfo>

#include "my_allocator.h"

template <typename T, std::size_t Capacity>
MyAllocator<T, Capacity>::MyAllocator() noexcept
    : m_storage(std::make_shared<AllocatorStorage<Capacity>>()) {
    std::cout << "default ctor: " << typeid(T).name() << "  storage:" << m_storage.get() << '\n';
}

template <typename T, std::size_t Capacity>
MyAllocator<T, Capacity>::MyAllocator(const MyAllocator& other) noexcept
    : m_storage(other.m_storage) {
    std::cout << "copy ctor same T: " << typeid(T).name() << "  storage:" << m_storage.get()
              << '\n';
}

template <typename T, std::size_t Capacity>
template <typename U>
MyAllocator<T, Capacity>::MyAllocator(const MyAllocator<U, Capacity>& other) noexcept
    : m_storage(other.m_storage) {
    std::cout << "copy ctor from U to T: " << typeid(U).name() << " -> " << typeid(T).name()
              << "  storage:" << m_storage.get() << '\n';
}

template <typename T, std::size_t Capacity>
MyAllocator<T, Capacity>::~MyAllocator() noexcept {
    std::cout << "dtor: " << typeid(T).name() << "  storage:" << m_storage.get() << '\n';
}

template <typename T, std::size_t Capacity>
T* MyAllocator<T, Capacity>::allocate(std::size_t n) {
    T* result = m_storage->template allocate<T>(n);
    std::cout << "allocate: " << typeid(T).name() << " n=" << n << " from "
              << static_cast<void*>(result) << "  storage:" << m_storage.get() << '\n';
    return result;
}

template <typename T, std::size_t Capacity>
void MyAllocator<T, Capacity>::deallocate(T* p, std::size_t n) noexcept {
    m_storage->template deallocate<T>(p, n);
}

template <typename T, std::size_t Capacity, typename U, std::size_t OtherCapacity>
bool operator==(const MyAllocator<T, Capacity>& lhs,
                const MyAllocator<U, OtherCapacity>& rhs) noexcept {
    if constexpr (Capacity != OtherCapacity) {
        return false;
    } else {
        return lhs.m_storage == rhs.m_storage;
    }
}

template <typename T, std::size_t Capacity, typename U, std::size_t OtherCapacity>
bool operator!=(const MyAllocator<T, Capacity>& lhs,
                const MyAllocator<U, OtherCapacity>& rhs) noexcept {
    return !(lhs == rhs);
}

#endif  // MY_ALLOCATOR_TPP
