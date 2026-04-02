#ifndef ALLOCATOR_STORAGE_TPP
#define ALLOCATOR_STORAGE_TPP

#include <iostream>
#include <new>
#include <stdexcept>
#include <typeinfo>

#include "allocator_storage.h"

template <std::size_t Capacity>
AllocatorStorage<Capacity>::AllocatorStorage() noexcept
    : m_memory(nullptr), m_used(0), m_element_size(0), m_element_align(0), m_initialized(false) {
    std::cout << "storage ctor: storage:" << this << '\n';
}

template <std::size_t Capacity>
AllocatorStorage<Capacity>::~AllocatorStorage() noexcept {
    std::cout << "storage dtor: storage:" << this << " memory:" << m_memory << " used:" << m_used
              << '\n';

    if (m_memory != nullptr) {
        ::operator delete(m_memory, std::align_val_t(m_element_align));
    }
}

template <std::size_t Capacity>
template <typename U>
U* AllocatorStorage<Capacity>::allocate(std::size_t n) {
    if (n == 0) {
        return nullptr;
    }

    ensureInitialized<U>();

    if (m_used + n > Capacity) {
        std::cout << "storage allocate failed: type=" << typeid(U).name() << " n=" << n
                  << " used=" << m_used << " capacity=" << Capacity << " storage:" << this << '\n';
        throw std::bad_alloc();
    }

    auto* typedMemory = static_cast<U*>(m_memory);
    U* result = typedMemory + m_used;
    std::cout << "storage allocate: type=" << typeid(U).name() << " n=" << n
              << " used_before=" << m_used;
    m_used += n;
    std::cout << " used_after=" << m_used << " from " << static_cast<void*>(result)
              << " storage:" << this << '\n';
    return result;
}

template <std::size_t Capacity>
template <typename U>
void AllocatorStorage<Capacity>::deallocate(U*, std::size_t) noexcept {
    // По условию задания поэлементное освобождение не требуется.
}

template <std::size_t Capacity>
template <typename U>
void AllocatorStorage<Capacity>::ensureInitialized() {
    if (!m_initialized) {
        m_element_size = sizeof(U);
        m_element_align = alignof(U);
        m_memory = ::operator new(Capacity * sizeof(U), std::align_val_t(alignof(U)));
        m_initialized = true;

        std::cout << "storage init: type=" << typeid(U).name() << " element_size=" << m_element_size
                  << " element_align=" << m_element_align << " capacity=" << Capacity
                  << " memory:" << m_memory << " - "
                  << static_cast<void*>(static_cast<char*>(m_memory) + Capacity * sizeof(U))
                  << " storage:" << this << '\n';
        return;
    }

    if (m_element_size != sizeof(U) || m_element_align != alignof(U)) {
        std::cout << "storage type mismatch: requested=" << typeid(U).name()
                  << " element_size=" << sizeof(U) << " element_align=" << alignof(U)
                  << " storage_size=" << m_element_size << " storage_align=" << m_element_align
                  << " storage:" << this << '\n';
        throw std::bad_alloc();
    }
}

#endif  // ALLOCATOR_STORAGE_TPP
