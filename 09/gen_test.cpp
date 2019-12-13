#include <iostream>
#include <fstream>

constexpr size_t CHUNK_SIZE = 1024*1024;
constexpr size_t UINT64_SIZE = sizeof(uint64_t);

int main(const int argc, const char **argv) {
	for (size_t i = 0; i < 10; ++i) {
		std::ofstream f;
		f.open("test" + std::to_string(i) + ".bin", std::ios::binary);
		for (size_t i = 0; i < 5 * CHUNK_SIZE; ++i) {
			uint64_t r = rand();
			f.write((char*)&r, UINT64_SIZE);
		}	
		f.close();
	}
	return 0;
}