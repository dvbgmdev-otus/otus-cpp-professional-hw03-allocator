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

    const auto elapsed = measureFactorialMapCreationTime<int, int>(itemCount, repeatCount);

    std::cout << "Average time: " << elapsed.count() / repeatCount << " ns\n";

    return 0;
}
