#include <string>

#include "app.h"

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--benchmark") {
        return runBenchmarkMode();
    }

    return runTaskMode();
}
