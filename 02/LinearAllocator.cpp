#include <cstring>
#include <cstdlib>

class Bad_Alloc{};

class LinearAllocator
{
    size_t len;
    char *begin;
    char *cur_ptr;
public:
    LinearAllocator(size_t maxSize): len(maxSize), begin(nullptr), cur_ptr(nullptr) 
    {
        begin = (char *)malloc(len);
        if (begin == nullptr) {
            throw Bad_Alloc();
        }
        cur_ptr = begin;
    }
    ~LinearAllocator() {
        if(begin != nullptr) {
            free(begin);
        }
    }
    char* alloc(size_t size) {
        if (len < size + cur_ptr - begin) {
            return nullptr;
        }
        char *tmp = cur_ptr;
        cur_ptr += size;
        return tmp;
    }
    void reset() {
        cur_ptr = begin;
    }
};
