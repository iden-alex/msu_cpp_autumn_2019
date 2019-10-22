#include <iostream>
#include "LinearAllocator.cpp"

int main() {
    LinearAllocator a(100);
    char *tmp1 = a.alloc(99);
    char *tmp2 = a.alloc(2);
    if (tmp2 == nullptr) {
        std::cout <<  true << std::endl;
    }
    return 0;
}
