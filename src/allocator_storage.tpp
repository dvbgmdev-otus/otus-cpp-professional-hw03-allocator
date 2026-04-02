#ifndef ALLOCATOR_STORAGE_TPP
#define ALLOCATOR_STORAGE_TPP

#include <new>
#include <stdexcept>

#include "allocator_storage.h"

template <std::size_t Capacity>
AllocatorStorage<Capacity>::AllocatorStorage() noexcept
    : m_memory(nullptr), m_used(0), m_element_size(0), m_element_align(0), m_initialized(false) {}

template <std::size_t Capacity>
AllocatorStorage<Capacity>::~AllocatorStorage() noexcept {
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
        throw std::bad_alloc();
    }

    auto* typedMemory = static_cast<U*>(m_memory);
    U* result = typedMemory + m_used;
    m_used += n;
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
        return;
    }

    if (m_element_size != sizeof(U) || m_element_align != alignof(U)) {
        throw std::bad_alloc();
    }
}

#endif  // ALLOCATOR_STORAGE_TPP
