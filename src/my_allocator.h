#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <cstddef>
#include <memory>

#include "allocator_storage.h"

template <typename T, std::size_t Capacity = 10>
class MyAllocator {
public:
    using value_type = T;

    MyAllocator() noexcept;
    MyAllocator(const MyAllocator&) noexcept;

    // Шаблонный конструктор, нужен для rebind
    template <typename U>
    MyAllocator(const MyAllocator<U, Capacity>&) noexcept;

    ~MyAllocator() noexcept;

    // Если не определять правила копирования, то лучше запретить. Иначе при копировании аллокатора
    // будет копироваться и его пул, что может привести к двойному освобождению памяти.
    MyAllocator& operator=(const MyAllocator&) = delete;

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n) noexcept;

    // rebind нужен потому, что std::map использует внутренний node-тип, отличный от value_type.
    template <typename U>
    struct rebind {
        using other = MyAllocator<U, Capacity>;
    };

private:
    template <typename U, std::size_t OtherCapacity>
    friend class MyAllocator;  // Чтобы разные инстанции шаблона могли видеть друг друга и при
                               // необходимости копировать свои пулы.

    std::shared_ptr<AllocatorStorage<Capacity>>
        m_storage;  // Общий блок памяти для всех аллокаторов с данным Capacity
};

template <typename T, std::size_t Capacity, typename U, std::size_t OtherCapacity>
bool operator==(const MyAllocator<T, Capacity>& lhs,
                const MyAllocator<U, OtherCapacity>& rhs) noexcept;

template <typename T, std::size_t Capacity, typename U, std::size_t OtherCapacity>
bool operator!=(const MyAllocator<T, Capacity>& lhs,
                const MyAllocator<U, OtherCapacity>& rhs) noexcept;

#include "my_allocator.tpp"

#endif  // MY_ALLOCATOR_H
