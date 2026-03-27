#include <cstddef>
#include <iostream>

#include "my_allocator.h"
#include "utils.h"

int main() {
    std::cout << "Hello, MyAllocator!" << std::endl;

    using MyMapAllocator = MyAllocator<std::pair<const int, int>>;

    const auto stdMap = createFactorialMap<int, int>(10);
    printMap(stdMap, std::cout, "Map with std::allocator:");

    const auto myAllocMap = createFactorialMap<int, int, std::less<int>, MyMapAllocator>(10);
    printMap(myAllocMap, std::cout, "Map with MyAllocator:");

    constexpr std::size_t repeatCount = 10000;
    constexpr std::size_t itemCount = 100;

    const auto stdMapElapsed = measureFactorialMapCreationTime<int, int>(itemCount, repeatCount);
    std::cout << "std::allocator average time: " << stdMapElapsed.count() / repeatCount << " ns\n";

    const auto myAllocMapElapsed =
        measureFactorialMapCreationTime<int, int, std::less<int>, MyMapAllocator>(itemCount,
                                                                                  repeatCount);
    std::cout << "MyAllocator average time: " << myAllocMapElapsed.count() / repeatCount << " ns\n";

    return 0;
}
