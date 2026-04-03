#include "app.h"

#include <cstddef>
#include <iostream>

#include "my_allocator.h"
#include "utils.h"

using MyMapAllocator = MyAllocator<std::pair<const int, int>, 10>;
using MyContainerAllocator = MyAllocator<int, 10>;

int runTaskMode() {
    const auto stdMap = createFactorialMap<int, int>(10);
    printMap(stdMap, "Map with std::allocator:");
    std::cout << '\n';

    const auto myAllocMap = createFactorialMap<int, int, std::less<int>, MyMapAllocator>(10);
    printMap(myAllocMap, "Map with MyAllocator:");
    std::cout << '\n';

    const auto stdContainer = createSequentialMyContainer<int>(10);
    printMyContainer(stdContainer, "MyContainer with std::allocator:");
    std::cout << '\n';

    const auto myAllocContainer = createSequentialMyContainer<int, MyContainerAllocator>(10);
    printMyContainer(myAllocContainer, "MyContainer with MyAllocator:");
    std::cout << '\n';

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

#ifdef ENABLE_LIST_EXPERIMENTS
    const auto stdList = createFactorialList<int>(10);
    std::cout << "List with std::allocator:\n";
    for (const auto& value : stdList) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    const auto myAllocList = createFactorialList<int, MyAllocator<int, 10>>(10);
    std::cout << "List with MyAllocator:\n";
    for (const auto& value : myAllocList) {
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

    const auto stdContainerElapsed = measureSequentialMyContainerCreationTime<int>(itemCount, repeatCount);
    std::cout << "std::allocator average time: " << stdContainerElapsed.count() / repeatCount << " ns\n";

    const auto myAllocContainerElapsed =
        measureSequentialMyContainerCreationTime<int, MyContainerAllocator>(itemCount, repeatCount);
    std::cout << "   MyAllocator average time: " << myAllocContainerElapsed.count() / repeatCount
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

#ifdef ENABLE_LIST_EXPERIMENTS
    const auto stdListElapsed = measureFactorialListCreationTime<int>(itemCount, repeatCount);
    std::cout << "std::allocator average time: " << stdListElapsed.count() / repeatCount << " ns\n";

    const auto myAllocListElapsed =
        measureFactorialListCreationTime<int, MyAllocator<int, 10>>(itemCount, repeatCount);
    std::cout << "   MyAllocator average time: " << myAllocListElapsed.count() / repeatCount
              << " ns\n";
#endif

    return 0;
}

