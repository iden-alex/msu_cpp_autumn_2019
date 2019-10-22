#include <iostream>
#include "LinearAllocator.cpp"

int main() {
    try {
        LinearAllocator a(1000000000000);
    } catch (Bad_Alloc &expr) {
        std::cout << "Bad_Alloc" << std::endl;
        return 1;
    }
    return 0;
}
