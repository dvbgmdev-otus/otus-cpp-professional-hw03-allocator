#include "app.h"

#include <cstddef>
#include <iostream>

#include "my_allocator.h"
#include "utils.h"

using MyMapAllocator = MyAllocator<std::pair<const int, int>, 10>;

int runTaskMode() {
    const auto stdMap = createFactorialMap<int, int>(10);
    printMap(stdMap, std::cout, "Map with std::allocator:");

    const auto myAllocMap = createFactorialMap<int, int, std::less<int>, MyMapAllocator>(10);
    printMap(myAllocMap, std::cout, "Map with MyAllocator:");

#ifdef ENABLE_VECTOR_EXPERIMENTS
    const auto stdVector = createFactorialVector<int>(10);
    std::cout << "Vector with std::allocator:\n";
    for (const auto& value : stdVector) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    const auto myAllocVector = createFactorialVector<int, MyAllocator<int, 10>>(10);
    std::cout << "Vector with MyAllocator:\n";
    for (const auto& value : myAllocVector) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
#endif

    return 0;
}

int runBenchmarkMode() {
    constexpr std::size_t repeatCount = 10000;
    constexpr std::size_t itemCount = 10;

    const auto stdMapElapsed = measureFactorialMapCreationTime<int, int>(itemCount, repeatCount);
    std::cout << "std::allocator average time: " << stdMapElapsed.count() / repeatCount << " ns\n";

    const auto myAllocMapElapsed =
        measureFactorialMapCreationTime<int, int, std::less<int>, MyMapAllocator>(itemCount,
                                                                                  repeatCount);
    std::cout << "   MyAllocator average time: " << myAllocMapElapsed.count() / repeatCount
              << " ns\n";

#ifdef ENABLE_VECTOR_EXPERIMENTS
    const auto stdVectorElapsed = measureFactorialVectorCreationTime<int>(itemCount, repeatCount);
    std::cout << "std::allocator average time: " << stdVectorElapsed.count() / repeatCount
              << " ns\n";

    const auto myAllocVectorElapsed =
        measureFactorialVectorCreationTime<int, MyAllocator<int, 10>>(itemCount, repeatCount);
    std::cout << "   MyAllocator average time: " << myAllocVectorElapsed.count() / repeatCount
              << " ns\n";
#endif

    return 0;
}
