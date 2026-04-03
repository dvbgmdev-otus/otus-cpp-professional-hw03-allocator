#ifndef MY_CONTAINER_TPP
#define MY_CONTAINER_TPP

#include "my_container.h"

// ****************************************************************************************
// Класс MyContainer - односвязный список с поддержкой аллокатора
// ****************************************************************************************

// Конструктор по умолчанию и с параметром размера
template <typename T, typename Allocator>
MyContainer<T, Allocator>::MyContainer(const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        push_back(T{});
    }
}

// Конструктор инициализации списком
template <typename T, typename Allocator>
MyContainer<T, Allocator>::MyContainer(const std::initializer_list<T> initList) {
    for (const auto& value : initList) {
        push_back(value);
    }
}

// Конструктор копирования
template <typename T, typename Allocator>
MyContainer<T, Allocator>::MyContainer(const MyContainer& other) {
    copyFrom(other);
}

// Конструктор перемещения
template <typename T, typename Allocator>
MyContainer<T, Allocator>::MyContainer(MyContainer&& other) noexcept {
    moveFrom(std::move(other));
}

// Оператор присваивания копированием
template <typename T, typename Allocator>
MyContainer<T, Allocator>& MyContainer<T, Allocator>::operator=(const MyContainer& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

// Оператор присваивания перемещением
template <typename T, typename Allocator>
MyContainer<T, Allocator>& MyContainer<T, Allocator>::operator=(MyContainer&& other) noexcept {
    if (this != &other) {
        clear();
        moveFrom(std::move(other));
    }
    return *this;
}

// Операторы сравнения
template <typename T, typename Allocator>
bool MyContainer<T, Allocator>::operator==(const MyContainer& other) const {
    // Если оба списка пустые - они равны
    if (m_size == 0 && other.m_size == 0) return true;

    // Если размеры разные - списки не равны
    if (m_size != other.m_size) return false;

    // Сравнение по элементам
    Node* a = m_firstNode;
    Node* b = other.m_firstNode;
    while (a && b) {
        if (a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    return true;
}

template <typename T, typename Allocator>
bool MyContainer<T, Allocator>::operator!=(const MyContainer& other) const {
    return !(*this == other);
}

// Деструктор
template <typename T, typename Allocator>
MyContainer<T, Allocator>::~MyContainer() {
    clear();
}

// --- Методы изменения списка ---

template <typename T, typename Allocator>
void MyContainer<T, Allocator>::push_back(const T& value) {
    Node* newNode = createNode(value);

    if (m_size == 0) {
        m_firstNode = newNode;
        m_lastNode = newNode;
    } else {
        m_lastNode->next = newNode;
        m_lastNode = newNode;
    }

    ++m_size;
}

template <typename T, typename Allocator>
void MyContainer<T, Allocator>::insert(const size_t index, const T& value) {
    if (index == m_size) {
        push_back(value);
        return;
    }
    checkIndex(index, "insert");
    Node* newNode = createNode(value);
    // вставка в начало
    if (index == 0) {
        newNode->next = m_firstNode;
        m_firstNode = newNode;
    } else {
        Node* prev = m_firstNode;
        for (size_t i = 0; i < index - 1; ++i) {
            prev = prev->next;
        }
        newNode->next = prev->next;
        prev->next = newNode;
    }
    ++m_size;
}

template <typename T, typename Allocator>
void MyContainer<T, Allocator>::erase(const size_t index) {
    checkIndex(index, "erase");
    Node* toDelete = nullptr;
    if (index == 0) {
        // Удаление первого элемента
        toDelete = m_firstNode;
        m_firstNode = m_firstNode->next;
        // Если список стал пустым
        if (m_firstNode == nullptr) {
            m_lastNode = nullptr;
        }
    } else {
        // Поиск узла, предшествующего удаляемому
        Node* prev = m_firstNode;
        for (size_t i = 0; i < index - 1; ++i) {
            prev = prev->next;
        }
        toDelete = prev->next;
        prev->next = toDelete->next;
        if (prev->next == nullptr) {
            m_lastNode = prev;
        }
    }
    destroyNode(toDelete);
    --m_size;
}

template <typename T, typename Allocator>
void MyContainer<T, Allocator>::clear() {
    Node* current = m_firstNode;
    while (current != nullptr) {
        Node* nextNode = current->next;
        destroyNode(current);
        current = nextNode;
    }
    m_firstNode = nullptr;
    m_lastNode = nullptr;
    m_size = 0;
}

// --- Методы информации о списке ---

template <typename T, typename Allocator>
typename MyContainer<T, Allocator>::size_type MyContainer<T, Allocator>::size() const {
    return m_size;
}

template <typename T, typename Allocator>
bool MyContainer<T, Allocator>::empty() const {
    return m_size == 0;
}

// --- Методы доступа к элементам ---

template <typename T, typename Allocator>
T& MyContainer<T, Allocator>::at(const size_t index) {
    checkIndex(index, "at");
    Node* current = m_firstNode;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template <typename T, typename Allocator>
const T& MyContainer<T, Allocator>::at(size_t index) const {
    checkIndex(index, "at");
    Node* current = m_firstNode;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template <typename T, typename Allocator>
T& MyContainer<T, Allocator>::operator[](const size_t index) {
    return at(index);
}

template <typename T, typename Allocator>
const T& MyContainer<T, Allocator>::operator[](const size_t index) const {
    return at(index);
}

// --- Итераторы ---

template <typename T, typename Allocator>
typename MyContainer<T, Allocator>::iterator MyContainer<T, Allocator>::begin() noexcept {
    return iterator(m_firstNode);
}

template <typename T, typename Allocator>
typename MyContainer<T, Allocator>::iterator MyContainer<T, Allocator>::end() noexcept {
    return iterator(nullptr);
}

template <typename T, typename Allocator>
typename MyContainer<T, Allocator>::const_iterator MyContainer<T, Allocator>::begin()
    const noexcept {
    return const_iterator(m_firstNode);
}

template <typename T, typename Allocator>
typename MyContainer<T, Allocator>::const_iterator MyContainer<T, Allocator>::end() const noexcept {
    return const_iterator(nullptr);
}

// --- Вывод в консоль ---

template <typename T, typename Allocator>
void MyContainer<T, Allocator>::print() const {
    Node* current = m_firstNode;
    size_t index = 0;
    std::cout << "=== List Contents ===" << std::endl;
    std::cout << " Size: " << m_size << std::endl;
    std::cout << "First: " << m_firstNode << std::endl;
    std::cout << " Last: " << m_lastNode << std::endl;
    std::cout << "---------------------" << std::endl;
    while (current != nullptr) {
        std::cout << "Node " << index++ << " " << current << ": data = " << current->data
                  << ", next = " << current->next << std::endl;
        current = current->next;
    }
    if (m_size == 0) {
        std::cout << "List is empty" << std::endl;
    }
}

// --- Приватные методы ---

template <typename T, typename Allocator>
typename MyContainer<T, Allocator>::Node* MyContainer<T, Allocator>::createNode(const T& value) {
    Node* node = node_allocator_traits::allocate(m_allocator, 1);
    try {
        node_allocator_traits::construct(m_allocator, node, value);
    } catch (...) {
        node_allocator_traits::deallocate(m_allocator, node, 1);
        throw;
    }
    return node;
}

template <typename T, typename Allocator>
typename MyContainer<T, Allocator>::Node* MyContainer<T, Allocator>::createNode(T&& value) {
    Node* node = node_allocator_traits::allocate(m_allocator, 1);
    try {
        node_allocator_traits::construct(m_allocator, node, std::move(value));
    } catch (...) {
        node_allocator_traits::deallocate(m_allocator, node, 1);
        throw;
    }
    return node;
}

template <typename T, typename Allocator>
void MyContainer<T, Allocator>::destroyNode(Node* node) noexcept {
    node_allocator_traits::destroy(m_allocator, node);
    node_allocator_traits::deallocate(m_allocator, node, 1);
}

template <typename T, typename Allocator>
void MyContainer<T, Allocator>::copyFrom(const MyContainer& other) {
    clear();
    Node* current = other.m_firstNode;
    while (current != nullptr) {
        push_back(current->data);
        current = current->next;
    }
}

template <typename T, typename Allocator>
void MyContainer<T, Allocator>::moveFrom(MyContainer&& other) noexcept {
    clear();
    m_size = other.m_size;
    m_firstNode = other.m_firstNode;
    m_lastNode = other.m_lastNode;
    other.m_size = 0;
    other.m_firstNode = nullptr;
    other.m_lastNode = nullptr;
}

template <typename T, typename Allocator>
void MyContainer<T, Allocator>::checkIndex(const size_t index, const char* function_name) const {
    if (index >= m_size) {
        std::ostringstream oss;
        oss << "MyContainer::" << function_name << "(): index " << index
            << " out of range (size: " << m_size << ")";
        throw std::out_of_range(oss.str());
    }
}

// ****************************************************************************************
// Класс MyContainer::Node - элемент односвязного списка
// ****************************************************************************************

template <typename T, typename Allocator>
MyContainer<T, Allocator>::Node::Node(const T& value) : next(nullptr), data(value) {}

template <typename T, typename Allocator>
MyContainer<T, Allocator>::Node::Node(T&& value) : next(nullptr), data(std::move(value)) {}

// *****************************************************************************************
// Класс MyContainer::iterator - итератор для MyContainer
// *****************************************************************************************

template <typename T, typename Allocator>
MyContainer<T, Allocator>::iterator::iterator(Node* node) : m_node(node) {}

template <typename T, typename Allocator>
T& MyContainer<T, Allocator>::iterator::operator*() const {
    return m_node->data;
}

template <typename T, typename Allocator>
T* MyContainer<T, Allocator>::iterator::get() const {
    return &m_node->data;
}

template <typename T, typename Allocator>
typename MyContainer<T, Allocator>::iterator& MyContainer<T, Allocator>::iterator::operator++() {
    if (m_node) {
        m_node = m_node->next;
    }
    return *this;
}

template <typename T, typename Allocator>
typename MyContainer<T, Allocator>::iterator MyContainer<T, Allocator>::iterator::operator++(int) {
    iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T, typename Allocator>
bool MyContainer<T, Allocator>::iterator::operator==(const iterator& other) const {
    return m_node == other.m_node;
}

template <typename T, typename Allocator>
bool MyContainer<T, Allocator>::iterator::operator!=(const iterator& other) const {
    return m_node != other.m_node;
}

// *****************************************************************************************
// Класс MyContainer::const_iterator - константный итератор для MyContainer
// *****************************************************************************************

template <typename T, typename Allocator>
MyContainer<T, Allocator>::const_iterator::const_iterator(const Node* node) : m_node(node) {}

template <typename T, typename Allocator>
const T& MyContainer<T, Allocator>::const_iterator::operator*() const {
    return m_node->data;
}

template <typename T, typename Allocator>
const T* MyContainer<T, Allocator>::const_iterator::get() const {
    return &m_node->data;
}

template <typename T, typename Allocator>
typename MyContainer<T, Allocator>::const_iterator&
MyContainer<T, Allocator>::const_iterator::operator++() {
    if (m_node) {
        m_node = m_node->next;
    }
    return *this;
}

template <typename T, typename Allocator>
typename MyContainer<T, Allocator>::const_iterator
MyContainer<T, Allocator>::const_iterator::operator++(int) {
    const_iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T, typename Allocator>
bool MyContainer<T, Allocator>::const_iterator::operator==(const const_iterator& other) const {
    return m_node == other.m_node;
}

template <typename T, typename Allocator>
bool MyContainer<T, Allocator>::const_iterator::operator!=(const const_iterator& other) const {
    return m_node != other.m_node;
}

// *****************************************************************************************
// Оператор вывода для MyContainer
// *****************************************************************************************
template <typename T, typename Allocator>
std::ostream& operator<<(std::ostream& os, const MyContainer<T, Allocator>& myContainer) {
    for (const auto& item : myContainer) {
        os << item << " ";
    }
    return os;
}

#endif  // MY_CONTAINER_TPP
