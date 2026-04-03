#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <cstddef>
#include <map>
#include <iostream>
#include <string>
#ifdef ENABLE_VECTOR_EXPERIMENTS
#include <vector>
#endif
#ifdef ENABLE_LIST_EXPERIMENTS
#include <list>
#endif

#include "my_container.h"

int factorial(int value);

// Функция для создания std::map с факториалами от 0 до itemCount - 1
template <typename Key,
          typename Value,
          typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<const Key, Value>>>
std::map<Key, Value, Compare, Allocator> createFactorialMap(
    std::size_t itemCount, const Allocator& allocator = Allocator()) {
    std::map<Key, Value, Compare, Allocator> result(Compare{}, allocator);
    for (std::size_t i = 0; i < itemCount; ++i) {
        result[static_cast<Key>(i)] =
            factorial(static_cast<int>(i) % 10);  // ограничиваем факториал до 9, чтобы избежать
                                                  // переполнения int при больших значениях i
    }
    return result;
}

// Функция для измерения времени создания N=repeatCount std::map с факториалами
template <typename Key,
          typename Value,
          typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<const Key, Value>>>
std::chrono::nanoseconds measureFactorialMapCreationTime(std::size_t itemCount,
                                                         std::size_t repeatCount) {
    std::size_t checksum = 0;
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < repeatCount; ++i) {
        Allocator allocator{};  // Создаем новый аллокатор для каждой итерации
        const auto tempMap =
            createFactorialMap<Key, Value, Compare, Allocator>(itemCount, allocator);
        checksum += tempMap.size();  // Используем размер map для предотвращения оптимизации
    }
    const auto end = std::chrono::steady_clock::now();
    (void)checksum;  // чтобы избежать предупреждения о неиспользуемой переменной
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

// Функция для печати содержимого std::map
template <typename Key, typename Value, typename Compare, typename Allocator>
void printMap(const std::map<Key, Value, Compare, Allocator>& map,
              const std::string& title = "") {
    if (!title.empty()) {
        std::cout << title << '\n';
    }
    for (const auto& [key, value] : map) {
        std::cout << key << " " << value << '\n';
    }
}

// Функция для создания MyContainer со значениями от 0 до itemCount - 1
template <typename Value, typename Allocator = std::allocator<Value>>
MyContainer<Value, Allocator> createSequentialMyContainer(
    std::size_t itemCount, const Allocator& allocator = Allocator()) {
    MyContainer<Value, Allocator> result(allocator);
    for (std::size_t i = 0; i < itemCount; ++i) {
        result.push_back(static_cast<Value>(i));
    }
    return result;
}

// Функция для измерения времени создания N=repeatCount MyContainer со значениями от 0 до itemCount
// - 1
template <typename Value, typename Allocator = std::allocator<Value>>
std::chrono::nanoseconds measureSequentialMyContainerCreationTime(std::size_t itemCount,
                                                                  std::size_t repeatCount) {
    std::size_t checksum = 0;
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < repeatCount; ++i) {
        Allocator allocator{};  // Создаем новый аллокатор для каждой итерации
        const auto tempContainer =
            createSequentialMyContainer<Value, Allocator>(itemCount, allocator);
        checksum +=
            tempContainer.size();  // Используем размер контейнера для предотвращения оптимизации
    }
    const auto end = std::chrono::steady_clock::now();
    (void)checksum;  // чтобы избежать предупреждения о неиспользуемой переменной
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

// Функция для печати содержимого MyContainer
template <typename Value, typename Allocator>
void printMyContainer(const MyContainer<Value, Allocator>& container,
                      const std::string& title = "") {
    if (!title.empty()) {
        std::cout << title << '\n';
    }
    for (const auto& item : container) {
        std::cout << item << ' ';
    }
    std::cout << '\n';
}

#ifdef ENABLE_VECTOR_EXPERIMENTS
// Функция для созданиия std::vector с факториалами от 0 до itemCount - 1
template <typename Value, typename Allocator = std::allocator<Value>>
std::vector<Value, Allocator> createFactorialVector(std::size_t itemCount,
                                                    const Allocator& allocator = Allocator()) {
    std::vector<Value, Allocator> result(allocator);
    result.reserve(itemCount);
    for (std::size_t i = 0; i < itemCount; ++i) {
        result.push_back(factorial(static_cast<int>(i) % 10));
    }
    return result;
}

// Функция для измерения времени создания N=repeatCount std::vector с факториалами
template <typename Value, typename Allocator = std::allocator<Value>>
std::chrono::nanoseconds measureFactorialVectorCreationTime(std::size_t itemCount,
                                                            std::size_t repeatCount) {
    std::size_t checksum = 0;
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < repeatCount; ++i) {
        Allocator allocator{};  // Создаем новый аллокатор для каждой итерации
        const auto tempVector = createFactorialVector<Value, Allocator>(itemCount, allocator);
        checksum += tempVector.size();  // Используем размер vector для предотвращения оптимизации
    }
    const auto end = std::chrono::steady_clock::now();
    (void)checksum;  // чтобы избежать предупреждения о неиспользуемой переменной
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}
#endif

#ifdef ENABLE_LIST_EXPERIMENTS
// Функция для создания std::list с факториалами от 0 до itemCount - 1
template <typename Value, typename Allocator = std::allocator<Value>>
std::list<Value, Allocator> createFactorialList(std::size_t itemCount,
                                                const Allocator& allocator = Allocator()) {
    std::list<Value, Allocator> result(allocator);
    for (std::size_t i = 0; i < itemCount; ++i) {
        result.push_back(factorial(static_cast<int>(i) % 10));
    }
    return result;
}

// Функция для измерения времени создания N=repeatCount std::list с факториалами
template <typename Value, typename Allocator = std::allocator<Value>>
std::chrono::nanoseconds measureFactorialListCreationTime(std::size_t itemCount,
                                                          std::size_t repeatCount) {
    std::size_t checksum = 0;
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < repeatCount; ++i) {
        Allocator allocator{};  // Создаем новый аллокатор для каждой итерации
        const auto tempList = createFactorialList<Value, Allocator>(itemCount, allocator);
        checksum += tempList.size();  // Используем размер list для предотвращения оптимизации
    }
    const auto end = std::chrono::steady_clock::now();
    (void)checksum;  // чтобы избежать предупреждения о неиспользуемой переменной
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}
#endif

#endif  // UTILS_H
