#include <chrono>
#include <cstddef>
#include <iostream>

#include "my_allocator.h"
#include "utils.h"

int main() {
    std::cout << "Hello, MyAllocator!" << std::endl;

    const auto myMap = createFactorialMap<int, int>(10);
    printMap(myMap, std::cout, "Factorials from 0 to 9:");

    constexpr std::size_t repeatCount = 10000;
    constexpr std::size_t itemCount = 100;

    auto timeCounter = std::chrono::nanoseconds::zero();
    std::size_t checksum = 0;
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < repeatCount; ++i) {
        const auto tempMap = createFactorialMap<int, int>(itemCount);
        checksum += tempMap.size();  // Используем размер map для предотвращения оптимизации
    }
    const auto end = std::chrono::steady_clock::now();
    timeCounter = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    (void)checksum;  // Используем checksum, чтобы избежать предупреждения о неиспользуемой переменной
    std::cout << "Average time: " << timeCounter.count() / repeatCount << " ns\n";

    return 0;
}
