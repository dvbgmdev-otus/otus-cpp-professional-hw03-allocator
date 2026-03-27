#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <cstddef>
#include <map>
#include <ostream>
#include <string>

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
                                                         std::size_t repeatCount,
                                                         const Allocator& allocator = Allocator()) {
    std::size_t checksum = 0;
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < repeatCount; ++i) {
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
              std::ostream& os,
              const std::string& title = "") {
    if (!title.empty()) {
        os << title << '\n';
    }
    for (const auto& [key, value] : map) {
        os << key << " " << value << '\n';
    }
}

#endif  // UTILS_H
