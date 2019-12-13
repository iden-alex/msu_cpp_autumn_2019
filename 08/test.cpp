#include <iostream>
#include <assert.h>
#include <chrono>
#include <thread>
#include "pool.cpp"

int f() {
	std::this_thread::sleep_for(std::chrono::seconds(3));
	return 0;
}
int main() {
    ThreadPool pool(4);
    auto t1 = pool.exec(f);
    for (int i = 0; i < 100; ++i) {
        assert(pool.exec([i]() { return i; }).get() == i);
    }
    assert(t1.get() == 0);
    std::cout << "Done" << std::endl;
    return 0;
}