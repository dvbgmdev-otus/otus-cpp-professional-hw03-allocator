#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <memory>

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

    // rebind нужен потому, что мапа 
    template <typename U>
    struct rebind {
        using other = MyAllocator<U, Capacity>;
    };

private:
    T* m_pool;
    std::size_t m_used;
};

template <typename T, std::size_t Capacity, typename U, std::size_t OtherCapacity>
bool operator==(const MyAllocator<T, Capacity>&, const MyAllocator<U, OtherCapacity>&) noexcept;

template <typename T, std::size_t Capacity, typename U, std::size_t OtherCapacity>
bool operator!=(const MyAllocator<T, Capacity>&, const MyAllocator<U, OtherCapacity>&) noexcept;

#include "my_allocator.tpp"

#endif  // MY_ALLOCATOR_H
