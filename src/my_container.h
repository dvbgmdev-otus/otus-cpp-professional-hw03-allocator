#ifndef MY_CONTAINER_H
#define MY_CONTAINER_H

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <stdexcept>

template <typename T>
class MyContainer {
public:
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
    bool operator!=(const MyContainer& other) const;       // оператора сравнения
    ~MyContainer();                                        // деструктор

    void push_back(const T& value);                   // добавление элемента в конец списка
    void insert(const size_t index, const T& value);  // добавление по индексу
    void erase(const size_t index);                   // удаление по индексу
    void clear();                                     // очистка списка
    size_t size() const;                              // текущий размер

    T& at(const size_t index);
    const T& at(size_t index) const;
    T& operator[](const size_t index);
    const T& operator[](const size_t index) const;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    void print() const;  // вывод в консоль

private:
    size_t m_size{ 0 };            // текущий размер
    Node* m_firstNode{ nullptr };  // указатель на первый элемент
    Node* m_lastNode{ nullptr };   // указатель на последний элемент

    void copyFrom(const MyContainer& other);      // копирование из другого списка
    void moveFrom(MyContainer&& other) noexcept;  // перемещение из другого списка
    void checkIndex(const size_t index, const char* function_name) const;  // проверка индекса
};

template <typename T>
class MyContainer<T>::Node {
public:
    Node* next{ nullptr };  // указатель на следующий элемент
    T data{};               // данные элемента
};

template <typename T>
class MyContainer<T>::iterator {
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

template <typename T>
class MyContainer<T>::const_iterator {
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

template <typename T>
std::ostream& operator<<(std::ostream& os, const MyContainer<T>& myVector);

#include "my_container.tpp"

#endif  // MY_CONTAINER_H
