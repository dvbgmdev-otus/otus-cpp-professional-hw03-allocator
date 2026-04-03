#ifndef MY_CONTAINER_H
#define MY_CONTAINER_H

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

// ****************************************************************************************
// Класс MyContainer - односвязный список с поддержкой аллокатора
// ****************************************************************************************
template <typename T, typename Allocator = std::allocator<T>>
class MyContainer {
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;

    class Node;

    class iterator;
    class const_iterator;

    explicit MyContainer(const size_t size = 0);  // конструктор по умолчанию и с параметром размера
    MyContainer(const std::initializer_list<T> initList);  // конструктор инициализации списком
    MyContainer(const MyContainer& other);                 // конструктор копирования
    MyContainer(MyContainer&& other) noexcept;             // конструктор перемещения
    MyContainer& operator=(const MyContainer& other);      // оператор присваивания копированием
    MyContainer& operator=(MyContainer&& other) noexcept;  // оператор присваивания перемещением
    bool operator==(const MyContainer& other) const;       // оператор сравнения
    bool operator!=(const MyContainer& other) const;       // оператор сравнения
    ~MyContainer();                                        // деструктор

    // --- Методы изменения списка ---
    void push_back(const T& value);                   // добавление элемента в конец списка
    void insert(const size_t index, const T& value);  // добавление по индексу
    void erase(const size_t index);                   // удаление по индексу
    void clear();                                     // очистка списка

    // --- Методы информации о списке ---
    size_type size() const;  // текущий размер
    bool empty() const;      // проверка на пустоту

    // --- Методы доступа к элементам ---
    T& at(const size_t index);
    const T& at(size_t index) const;
    T& operator[](const size_t index);
    const T& operator[](const size_t index) const;

    // --- Итераторы ---
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    void print() const;  // вывод в консоль

private:
    using allocator_traits = std::allocator_traits<allocator_type>;
    using node_allocator_type = typename allocator_traits::template rebind_alloc<Node>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;

    size_type m_size{ 0 };              // текущий размер
    Node* m_firstNode{ nullptr };       // указатель на первый элемент
    Node* m_lastNode{ nullptr };        // указатель на последний элемент
    node_allocator_type m_allocator{};  // аллокатор для узлов списка

    Node* createNode(const T& value);
    Node* createNode(T&& value);
    void destroyNode(Node* node) noexcept;

    void copyFrom(const MyContainer& other);      // копирование из другого списка
    void moveFrom(MyContainer&& other) noexcept;  // перемещение из другого списка
    void checkIndex(const size_t index, const char* function_name) const;  // проверка индекса
};

// ****************************************************************************************
// Класс MyContainer::Node - элемент односвязного списка
// ****************************************************************************************
template <typename T, typename Allocator>
class MyContainer<T, Allocator>::Node {
public:
    Node* next{ nullptr };  // указатель на следующий элемент
    T data{};               // данные элемента

    Node() = default;

    explicit Node(const T& value);
    explicit Node(T&& value);
};

// *****************************************************************************************
// Класс MyContainer::iterator - итератор для MyContainer
// *****************************************************************************************
template <typename T, typename Allocator>
class MyContainer<T, Allocator>::iterator {
public:
    explicit iterator(Node* node = nullptr);
    T& operator*() const;
    T* get() const;
    iterator& operator++();
    iterator operator++(int);
    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;

private:
    Node* m_node;
};

// *****************************************************************************************
// Класс MyContainer::const_iterator - константный итератор для MyContainer
// *****************************************************************************************
template <typename T, typename Allocator>
class MyContainer<T, Allocator>::const_iterator {
public:
    explicit const_iterator(const Node* node = nullptr);
    const T& operator*() const;
    const T* get() const;
    const_iterator& operator++();
    const_iterator operator++(int);
    bool operator==(const const_iterator& other) const;
    bool operator!=(const const_iterator& other) const;

private:
    const Node* m_node;
};

// *****************************************************************************************
// Оператор вывода для MyContainer
// *****************************************************************************************
template <typename T, typename Allocator>
std::ostream& operator<<(std::ostream& os, const MyContainer<T, Allocator>& myContainer);

#include "my_container.tpp"

#endif  // MY_CONTAINER_H
