#include <cstdlib>

class Row {
    size_t cols;
    int *ptr;
public:    
    Row(size_t n, int *arr_row): cols(n), ptr(arr_row) {}
    ~Row() {}
    int operator [](size_t j) const {
        if (j < cols) {
            return *(ptr + j);
        } else {
            throw std::out_of_range("");
        }
    }
    int& operator [](size_t j) {
        if (j < cols) {
            return *(ptr + j);
        } else {
            throw std::out_of_range("");
        }
    }
};

class Matrix {
    size_t rows;
    size_t cols;
    int *ptr;
public:
    Matrix(size_t r, size_t c):  rows(r), cols(c), ptr(new int[r*c]) {}
    ~Matrix() {
        if (ptr != nullptr) {
            delete [] ptr;
        }
    }
    Matrix(const Matrix &B): rows(B.getRows()), cols(B.getColumns()),
        ptr(new int[rows*cols]) 
    {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                ptr[i*cols + j] = B.getElem(i, j);
            }
        }
        
    }
    Matrix& operator =(const Matrix &B) {
        if (this == &B) {
            return *this;
        }
        if (ptr != nullptr) {
            free(ptr);
        }
        cols = B.getColumns();
        rows = B.getRows();
        ptr = new int[rows*cols];
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                ptr[i*cols + j] = B.getElem(i, j);
            }
        }
        return *this;
    }
    int getElem(int i, int j) const {
        return ptr[i*cols + j];
    }
    size_t getRows() const {
        return rows;
    }
    size_t getColumns() const {
        return cols;
    }
    const Row operator[](size_t i) const {
        if (i < rows) {
            return Row(cols, ptr + i*cols);
        } else {
            throw std::out_of_range("");
        }
    }
    Row operator[](size_t i) {
        if (i < rows) {
            return Row(cols, ptr + i*cols);
        } else {
            throw std::out_of_range("");
        }
    }
    Matrix& operator *=(const int k) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                ptr[i*cols + j] *= k;
            }
        }
        return *this;
    }
    bool operator ==(const Matrix &B) const {
        if (this == &B) {
            return true;
        }
        if (rows != B.getRows() || cols != B.getColumns()) {
            return false;
        } 
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (ptr[i*cols + j] != B.getElem(i, j)) {
                    return false;
                }
            }
        }
    }
    bool operator !=(const Matrix &B) const {
        return !(*this == B);
    }
};
