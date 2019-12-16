#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <pthread.h>
#include <algorithm>

constexpr size_t CHUNK_SIZE = 1024*1024;
constexpr size_t UINT64_SIZE = sizeof(uint64_t);

size_t file_len(std::ifstream &f) {
	f.seekg (0, f.end);
	size_t len = f.tellg() / UINT64_SIZE;
	f.seekg(0, f.beg);
	return len;
}

int main(const int argc, const char **argv) {
	std::ifstream input(argv[1], std::ios::binary);
	if (!input){
        std::cerr << "Can't open " << argv[1] << '\n';
        return 1;
    }
	size_t len = file_len(input);
	uint64_t* arr = new uint64_t [CHUNK_SIZE];
	size_t num_files = 2*(len/CHUNK_SIZE + (bool)(len % CHUNK_SIZE));
	std::vector <std::string> f_names(num_files);
	for (size_t i = 0; i < num_files; ++i) {
		f_names[i] = std::to_string(i) + ".bin";
	}
	for (size_t i = 0; !input.eof(); i += 2){
		input.read((char*)arr, CHUNK_SIZE * UINT64_SIZE );
		size_t k = input.gcount() / UINT64_SIZE;
		if (k) {
			std::thread t1([arr, k]() { 
				std::sort(arr, arr + k/2);
			});
			std::thread t2([arr, k]() { 
				std::sort(arr + k/2, arr + k);
			});
			t1.join();
			t2.join();
			std::ofstream f1(f_names[i], std::ios::binary);
			if (!f1) {
        		std::cerr << "Can't open " << f_names[i] << '\n';
        		return 1;
    		}
			std::ofstream f2(f_names[i+1], std::ios::binary);
			if (!f2) {
        		std::cerr << "Can't open " << f_names[i+1] << '\n';
        		return 1;
    		}
			for (size_t m = 0; m < k/2;++m) {
				f1.write((char *)(arr+m), UINT64_SIZE);
			}
			for (size_t m = k/2; m < k;++m) {
				f2.write((char *)(arr+m), UINT64_SIZE);
			}
			f1.close();
			f2.close();
		}
	}
	input.close();
	for (size_t i = 0; i < num_files - 1; ++i) {
		std::ifstream fr(f_names[i+1], std::ios::binary);
		if (!fr) {
        	std::cerr << "Can't open " << f_names[i+1] << '\n';
        	return 1;
    	}
		size_t k = file_len(fr);
		fr.read((char*)arr, k * UINT64_SIZE);
		fr.close();
		std::ifstream merg_f(f_names[i], std::ios::binary);
		if (!merg_f) {
        	std::cerr << "Can't open " << f_names[i] << '\n';
        	return 1;
    	}
		std::ofstream res(f_names[i+1], std::ios::binary);
		if (!res) {
        	std::cerr << "Can't open " << f_names[i+1] << '\n';
        	return 1;
    	}
		size_t pos = 0;
		uint64_t a;
		merg_f.read((char*)&a, UINT64_SIZE);

		while(!merg_f.eof() && pos < k) {
			if (a < arr[pos]) {
				res.write((char*)&a, UINT64_SIZE );
				merg_f.read((char*)&a, UINT64_SIZE );
			} else if (a > arr[pos]) {
				res.write((char*)(arr + pos), UINT64_SIZE );
				pos++;
			} else {
				res.write((char*)&a, UINT64_SIZE );
				res.write((char*)(arr + pos), UINT64_SIZE);
				pos++;
				merg_f.read((char*)&a, UINT64_SIZE);
			}
		}
		if (merg_f.eof() && pos >= k) {
			merg_f.close();
			res.close();
			continue;
		} else if (pos >= k) {
			while(!merg_f.eof()) {
				res.write((char*)&a, UINT64_SIZE);
				merg_f.read((char*)&a, UINT64_SIZE);
			}
		} else if (merg_f.eof()) {
			for (size_t j = pos; pos < k; ++pos) {
				res.write((char*)(arr + j), UINT64_SIZE);
			}
		}
		merg_f.close();
		res.close();
	}
	delete [] arr;
	std::rename(f_names[num_files - 1].c_str(), "output.bin");
	for (size_t i = 0; i < num_files - 1; ++i) {
		std::remove(f_names[i].c_str());
	}
	return 0;
}