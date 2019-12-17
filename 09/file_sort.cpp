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

std::vector <std::string> input_splitting(const char* file_name) {
	std::ifstream input(file_name, std::ios::binary);
	if (!input){
        	throw std::ios_base::failure(file_name);
    	}
	size_t len = file_len(input);
	uint64_t* buf = new uint64_t [CHUNK_SIZE];
	size_t num_files = 2 * (len / CHUNK_SIZE + (bool)(len % CHUNK_SIZE));
	
	std::vector <std::string> f_names(num_files);
	for (size_t i = 0; i < num_files; ++i) {
		f_names[i] = std::to_string(i) + ".bin";
	}

	for (size_t i = 0; !input.eof(); i += 2){
		input.read((char*)buf, CHUNK_SIZE * UINT64_SIZE);
		size_t k = input.gcount() / UINT64_SIZE;
		if (k) {
			std::thread t1([buf, k]() { 
				std::sort(buf, buf + k/2);
			});
			std::thread t2([buf, k]() { 
				std::sort(buf + k/2, buf + k);
			});
			t1.join();
			t2.join();
			std::ofstream f1(f_names[i], std::ios::binary);
			if (!f1) {
				delete [] buf;
        			throw std::ios_base::failure(f_names[i]);
    			}
			std::ofstream f2(f_names[i+1], std::ios::binary);
			if (!f2) {
				delete [] buf;
        			throw std::ios_base::failure(f_names[i+1]);
    			}
			for (size_t m = 0; m < k/2;++m) {
				f1.write((char *)(buf + m), UINT64_SIZE);
			}
			for (size_t m = k/2; m < k;++m) {
				f2.write((char *)(buf + m), UINT64_SIZE);
			}
			f1.close();
			f2.close();
		}
	}
	delete [] buf;
	input.close();
	return f_names;
}

void file_merge(std::vector <std::string> &f_names) {
	uint64_t* buf = new uint64_t [CHUNK_SIZE];
	size_t num_files = f_names.size();
	for (size_t i = 0; i < num_files - 1; ++i) {
		std::ifstream fr(f_names[i+1], std::ios::binary);
		if (!fr) {
			delete [] buf;
	       		throw std::ios_base::failure(f_names[i+1]);
	   	}
		size_t k = file_len(fr);
		fr.read((char*)buf, k * UINT64_SIZE);
		fr.close();

		std::ifstream merg_f(f_names[i], std::ios::binary);
		if (!merg_f) {
			delete [] buf;
	       		throw std::ios_base::failure(f_names[i]);
	    	}
		std::ofstream res(f_names[i+1], std::ios::binary);
		if (!res) {
			delete [] buf;
	        	throw std::ios_base::failure(f_names[i+1]);
	    	}

		size_t pos = 0;
		uint64_t tmp;
		merg_f.read((char*)&tmp, UINT64_SIZE);
		while(!merg_f.eof() && pos < k) {
			if (tmp < buf[pos]) {
				res.write((char*)&tmp, UINT64_SIZE );
				merg_f.read((char*)&tmp, UINT64_SIZE );
			} else if (tmp > buf[pos]) {
				res.write((char*)(buf + pos), UINT64_SIZE );
				pos++;
			} else {
				res.write((char*)&tmp, UINT64_SIZE );
				res.write((char*)(buf + pos), UINT64_SIZE);
				pos++;
				merg_f.read((char*)&tmp, UINT64_SIZE);
			}
		}

		if (merg_f.eof() && pos >= k) {
			merg_f.close();
			res.close();
			continue;
		} else if (pos >= k) {
			while(!merg_f.eof()) {
				res.write((char*)&tmp, UINT64_SIZE);
				merg_f.read((char*)&tmp, UINT64_SIZE);
			}
		} else if (merg_f.eof()) {
			for (size_t j = pos; pos < k; ++pos) {
				res.write((char*)(buf + j), UINT64_SIZE);
			}
		}
		merg_f.close();
		res.close();
	}
	delete [] buf;
	std::rename(f_names[num_files - 1].c_str(), "output.bin");	
}

int main(const int argc, const char **argv) {
	try {
		auto f_names = input_splitting(argv[1]);
		file_merge(f_names);
		for (size_t i = 0; i < f_names.size() - 1; ++i) {
			std::remove(f_names[i].c_str());
		}
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}
