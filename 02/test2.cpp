#include <iostream>
#include "LinearAllocator.cpp"

int main() {
    LinearAllocator a(100);
    char *tmp1 = a.alloc(10);
    for (auto i: "123456789") {
        *tmp1 = i;
        tmp1++;
    }
    char *tmp2 = a.alloc(80);
    a.reset();
    char *tmp3 = a.alloc(10);
    std::cout << tmp3 << std::endl;
    return 0;
} 
