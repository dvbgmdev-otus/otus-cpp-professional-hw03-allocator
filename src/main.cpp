#include <iostream>
#include <map>

#include "my_allocator.h"
#include "utils.h"

int main() {
    std::cout << "Hello, MyAllocator!" << std::endl;

    // создание экземпляра std::map<int, int>
    std::map<int, int> myMap;

    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    for (int i = 0; i < 10; ++i) {
        myMap[i] = factorial(i);
    }

    // вывод содержимого map
    for (const auto& [key, value] : myMap) {
        std::cout << key << " " << value << std::endl;
    }

    return 0;
}
