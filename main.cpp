#include "MemorySimulator.h"

int main() {
    int numFrames = 3;
    MemorySimulator simulator(numFrames);

    std::vector<int> pagesToAccess = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};

    for (int page : pagesToAccess) {
        std::cout << "Accessing page: " << page << std::endl;
        simulator.accessPage(page);
        simulator.displayFrames();
    }

    return 0;
}
