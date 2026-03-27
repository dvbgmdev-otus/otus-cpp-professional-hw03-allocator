#ifndef MY_ALLOCATOR_TPP
#define MY_ALLOCATOR_TPP

#include "my_allocator.h"
#include <iostream>

template <typename T>
template <typename U>
MyAllocator<T>::MyAllocator(const MyAllocator<U>&) {}

template <typename T>
T* MyAllocator<T>::allocate(std::size_t n) {
    std::cout << "Allocating " << n << " element(s). Total allocation size: " << n * sizeof(T) << " bytes." << std::endl;
    return static_cast<T*>(::operator new(n * sizeof(T)));
}

template <typename T>
void MyAllocator<T>::deallocate(T* p, std::size_t n) {
    std::cout << "Deallocating " << n << " element(s). Total deallocation size: " << n * sizeof(T) << " bytes." << std::endl;
    ::operator delete(p);
}

template <typename T, typename U>
bool operator==(const MyAllocator<T>&, const MyAllocator<U>&) noexcept{
    return true;
}

template <typename T, typename U>
bool operator!=(const MyAllocator<T>&, const MyAllocator<U>&) noexcept{
    return false;
}

#endif  // MY_ALLOCATOR_TPP
