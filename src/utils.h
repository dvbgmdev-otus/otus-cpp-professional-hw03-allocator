#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <ostream>
#include <string>

int factorial(int value);

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
