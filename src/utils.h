#ifndef UTILS_H
#define UTILS_H

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
