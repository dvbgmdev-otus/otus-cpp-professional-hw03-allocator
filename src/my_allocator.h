#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <memory>

template <typename T>
class MyAllocator {
public:
    using value_type = T;

    MyAllocator() = default;

    template <typename U>
    MyAllocator(const MyAllocator<U>&);

    T* allocate(std::size_t n);

    void deallocate(T* p, std::size_t);
};

template <typename T, typename U>
bool operator==(const MyAllocator<T>&, const MyAllocator<U>&) noexcept;

template <typename T, typename U>
bool operator!=(const MyAllocator<T>&, const MyAllocator<U>&) noexcept;

#include "my_allocator.tpp"

#endif  // MY_ALLOCATOR_H
