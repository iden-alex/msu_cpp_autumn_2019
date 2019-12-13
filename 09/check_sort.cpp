#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int main(const int argc, const char **argv) {
	std::ifstream f("output.bin", std::ios::binary);
	f.seekg (0, f.end);
	uint64_t len = f.tellg() / sizeof(uint64_t);
	f.seekg(0, f.beg);
	uint64_t* arr = new uint64_t [len+1];
	f.read((char*)arr, len *sizeof(uint64_t));
	if (std::is_sorted(arr, arr + len)) {
		std::cout << "output.bin is sorted" << std::endl;
	} else {
		std::cout << "output.bin is not sorted" << std::endl;
	}
	delete [] arr;
	f.close();
	return 0;
}