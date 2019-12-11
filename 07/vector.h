 #ifndef MY_VECTOR
#define MY_VECTOR

template <class T>
class Allocator
{
public:
    Allocator() {}
    ~Allocator() {}
    using value_type = T;
    using pointer = T*;

    pointer allocate(size_t count) {
        //return new value_type[count];
        if (count == 0) {
            return nullptr;
        }
        return static_cast<pointer>(new value_type[count]);
    }
    void deallocate(pointer ptr, size_t count) {
        delete [] ptr;
    }
    size_t max_size() const noexcept {
        std::numeric_limits<size_t>::max();
    }
};

template <class T>
class Iterator {
    T* ptr_;
    bool rev_f;
public:
    using reference = T&;
    explicit Iterator(T* ptr, bool rev = false) : ptr_(ptr), rev_f(rev) {}

    Iterator& operator++() {
        if (rev_f) {
            --ptr_;
        } else {
            ++ptr_;
        }
        return *this;
    }

    Iterator& operator+(size_t n) {
        if (rev_f) {
            ptr_ -= n;
        } else {
            ptr_ += n;
        }
        return *this;
    }

    Iterator& operator-(size_t n) {
        if (rev_f) {
            ptr_ += n;
        } else {
            ptr_ -= n;
        }
        return *this;
    }

    Iterator& operator--() {
        if (rev_f) {
            ptr_++;
        } else {
            ptr_--;
        }
        return *this;
    }
    
    bool operator==(const Iterator<T>& b) const {
        return ptr_ == b.ptr_;
    }

    bool operator!=(const Iterator<T>& b) const {
        return !(*this == b);
    }

    reference operator*() const {
        return *ptr_;
    }

};

template <class T, class Alloc = Allocator<T>>
class Vector
{
    size_t size_;
    Alloc alloc_;
    T* ptr;
    size_t capacity_;
public:
    using iterator = Iterator<T>;
    Vector(size_t s, T& default_val = T()): size_(s), alloc_(Alloc()), ptr(alloc_.allocate(s)), capacity_(s)
    {
        for (size_t i = 0; i < s; i++) {
            ptr[i] = default_val;
        }
    }
    Vector():  size_(0), ptr(alloc_.allocate(0)), capacity_(0)
    {

    }
    ~Vector() {
        clear();
        alloc_.deallocate(ptr, capacity_);
    }
    
    void resize(size_t new_size) {
        if (new_size < size_) {
            for (size_t i = new_size; i < size_; ++i) {
                ptr[i].~T();
            }
            size_ = new_size;
        } else if (new_size > size_) {
            if (new_size > capacity_) {
                reserve(new_size);
            }
            for (size_t i = size_; i < new_size; ++i) {
                ptr[i] = T();
            }
            size_ = new_size;
        }
    }
    
    void reserve(size_t new_cap) {
        if (new_cap > capacity_) {
            T* new_ptr = alloc_.allocate(new_cap);
            for (size_t i = 0; i < size_; ++i) {
                new (new_ptr + i) T (ptr[i]);
                //new_ptr[i] = ptr[i];
                ptr[i].~T();
            }
            alloc_.deallocate(ptr, capacity_);
            ptr = new_ptr;
            capacity_ = new_cap;
        }
    }
    
    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            ptr[i].~T();
        }
        size_ = 0;
    }
    
    void push_back(const T& val) {
        if (capacity_ > size_) {
            size_++;
        } else {
            resize(size_+1);
        }
        ptr[size_ - 1] = std::move(val);
    }
    
    void pop_back() {
        if (size_ > 0) {
            size_ --;
            ptr[size_].~T();
        }
    }
    
    T operator[](size_t i) const {
        if (i >= size_) {
            throw std::out_of_range("Invalid index");
        }
        return ptr[i];
    }
    
    T& operator[](size_t i) {
        if (i >= size_) {
            throw std::out_of_range("Invalid index");
        }
        return ptr[i];
    }
    
    
    size_t size() const {
        return size_;
    }
        
    size_t capacity() const {
        return capacity_;
    }
    
    bool empty() const {
        return (size_ == 0);
    }
    
    iterator begin() {
        return Iterator<T>(ptr);
    }
    
    iterator end() {
        return Iterator<T>(ptr + size_);
    }
    
    iterator rbegin() {
        return Iterator<T>(ptr + size_ - 1, true);
    }
    
    iterator rend() {
        return Iterator<T>(ptr - 1, true);
    }
};
 
 #endif
