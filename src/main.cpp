#include <iostream>
#include "my_allocator.h"
#include <list>

int main() {
    std::cout << "Hello, MyAllocator!" << std::endl;

    std::list<int, MyAllocator<int>> list;
    list.push_back(1);

    for (const auto& val : list) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
