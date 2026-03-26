#include <iostream>
#include "my_allocator.h"
#include <vector>

int main() {
    std::cout << "Hello, MyAllocator!" << std::endl;

    std::vector<int, MyAllocator<int>> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (const auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
