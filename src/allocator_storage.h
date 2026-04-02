#ifndef ALLOCATOR_STORAGE_H
#define ALLOCATOR_STORAGE_H

#include <cstddef>

// Класс для хранения общего блока памяти, который будет использоваться всеми аллокаторами.
template <std::size_t Capacity>
class AllocatorStorage {
public:
    explicit AllocatorStorage() noexcept;
    ~AllocatorStorage() noexcept;

    AllocatorStorage(const AllocatorStorage&) = delete;
    AllocatorStorage& operator=(const AllocatorStorage&) = delete;

    template <typename U>
    U* allocate(std::size_t n);

    template <typename U>
    void deallocate(U* p, std::size_t n) noexcept;

private:
    template <typename U>
    void
    ensureInitialized();  // Инициализирует storage для типа U при первом использовании. При
                          // повторном использовании проверяет совместимость размера и выравнивания.

private:
    void* m_memory;               // адрес ообщего сырого блока
    std::size_t m_used;           // сколько элементов уже выделено
    std::size_t m_element_size;   // размер фактчески выделяемого элемента
    std::size_t m_element_align;  // выравнивание фактчески выделяемого элемента
    bool m_initialized;           // storage уже привязан к конкретному типу или нет
};

#include "allocator_storage.tpp"

#endif  // ALLOCATOR_STORAGE_H
