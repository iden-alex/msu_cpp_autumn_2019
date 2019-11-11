 #include <iostream>
 
class BigInt {
    size_t len;
    bool sign;
    char *ptr;
public:
    BigInt(long long a = 0):len(0), sign(a >= 0), ptr(nullptr) {
        if (!a) {
            len = 1;
            ptr = new char[1];
            ptr[0] = 0;
        } else {
            char num[19];
            long long tmp = a;
            if (!sign) {
                tmp = -tmp;
            }
            while (tmp) {
                num[len] = tmp % 10;
                tmp /= 10;
                len++;
            }
            ptr = new char[len];
            for (size_t i = 0; i < len; ++i) {
                ptr[i] = num[i];
            }
        }
    }
    
    BigInt(const BigInt &b):len(b.len), sign(b.sign), ptr(new char[b.len])
    {
        
        for (size_t i = 0; i < len; ++i) {
            ptr[i] = b.ptr[i];
        }
    }
    
    ~BigInt() {
        delete [] ptr;
    }
    bool Is_positive() const {
        return sign;
    }
    int GetLen() const {
        return len;
    }
    
    BigInt& operator=(long long a) {
        BigInt tmp(a);
        len = tmp.len;
        sign = tmp.sign;
        delete [] ptr;
        ptr = new char[len];
        for (size_t i = 0; i < len; ++i) {
            ptr[i] = tmp.ptr[i];
        }
        return *this;
    }
    
    BigInt& operator=(const BigInt& b) {
        len = b.len;
        sign = b.sign;
        delete [] ptr;
        ptr = new char[len];
        for (size_t i = 0; i < len; ++i) {
            ptr[i] = b.ptr[i];
        }
        return *this;
    }
    
    BigInt operator -() const {
        if (len == 1 && !ptr[0]) {
            return *this;
        }
        BigInt result(*this);
        result.sign = !sign;
        return result;
    }
    
    BigInt Add_pos_arg(const BigInt &b) const {
        if (len >= b.len) {
            bool diff = false;
            for (size_t i = 0; i < len; ++i) {
                char term = i < b.len? b.ptr[i] : 0;
                diff = (ptr[i] + term + diff) > 9;
            }
            BigInt res;
            res.len = len + diff;
            res.sign = true;
            res.ptr = new char[res.len];
            diff = false;
            for (size_t i = 0; i < res.len; ++i) {
                char term_a = i < b.len? b.ptr[i] : 0;
                char term_b = i < len? ptr[i] : 0;
                res.ptr[i] = (term_a + term_b + diff) %10;
                diff = (term_a + term_b + diff) / 10;
            }
            return res;
        } else {
            return b.Add_pos_arg(*this);
        }
    }
    
    BigInt Add_neg_arg(const BigInt &b) const {
        size_t k = 0;
        bool diff = false;
        char buf[len + 1];
        for (size_t i = 0; i < len; ++i) {
            char term_b = i < b.len? b.ptr[i] : 0;
            char num = ptr[i] - term_b - diff;
            diff = num < 0;
            if (num != 0 && num != -10) {
                k = i + 1;
            }
            buf[i] = diff? 10 + num: num;
        }
        if (!k) {
            return BigInt();
        }
        BigInt res;
        res.len = k;
        res.sign = true;
        res.ptr = new char[k];
        for (size_t i = 0; i < res.len; ++i) {
            res.ptr[i] = buf[i];
        }
        return res;
    }
    
    bool Is_mod_greater(const BigInt &b) const {
        
        if (len == b.len) {
            for (size_t i = 1; i <= len; ++i) {
                if (ptr[len - i] > b.ptr[len - i]) {
                    return true;
                } 
                if (ptr[len - i] < b.ptr[len - i]) {
                    return false;
                } 
            }
            return false;
        } else if (len < b.len){
            return false;
        } else {
            return true;
        }
    }
    
    friend BigInt operator +(const BigInt&, const BigInt&);
    friend BigInt operator -(const BigInt&, const BigInt&);
    friend bool operator ==(const BigInt&, const BigInt&);
    friend bool operator !=(const BigInt&, const BigInt&);
    friend bool operator <(const BigInt&, const BigInt&);
    friend bool operator <=(const BigInt&, const BigInt&);
    friend bool operator >(const BigInt&, const BigInt&);
    friend bool operator >=(const BigInt&, const BigInt&);
    friend std::ostream& operator<<(std::ostream&, const BigInt&);
};

BigInt operator +(const BigInt& a, const BigInt &b) {
    if (a.sign == b.sign) {
        if (a.sign) {
            return a.Add_pos_arg(b);
        }
        return -a.Add_pos_arg(b);
    } else {
        if (a.sign) {
            if (a.Is_mod_greater(b)) {
                return a.Add_neg_arg(b);
            }
            return -b.Add_neg_arg(a);
        } else {
            if (a.Is_mod_greater(b)) {
                return -a.Add_neg_arg(b);
            }
            return b.Add_neg_arg(a);
        }
    }
}

BigInt operator -(const BigInt& a, const BigInt &b) {
    return a + (-b);
}

std::ostream& operator<<(std::ostream& os, const BigInt& a) {
    if (!a.sign) 
        os << '-';
    for(size_t i = 1; i <= a.len; i++) {
        char c = a.ptr[a.len - i] + '0';
        os << c;
    }
    return os;
}

bool operator ==(const BigInt &a, const BigInt &b) {
    if (a.len != b.len || a.sign != b.sign) {
        return false;
    }
    for (size_t i = 0; i < a.len; ++i) {
        if (a.ptr[i] != b.ptr[i]) {
            return false;
        }
    }
    return true;
}

bool operator !=(const BigInt &a, const BigInt &b) {
    if (a == b) {
        return false;
    } else {
        return true;
    }
}

bool operator >(const BigInt &a, const BigInt &b) {
    if (a == b) {
        return false;
    }
    BigInt diff = a - b;
    if (diff.sign) {
        return true;
    } else {
        return false;
    }
}

bool operator >=(const BigInt &a, const BigInt &b) {
    if (a == b) {
        return true;
    }
    BigInt diff = a - b;
    if (diff.sign) {
        return true;
    } else {
        return false;
    }
}

bool operator <(const BigInt &a, const BigInt &b) {
    if (a == b) {
        return false;
    }
    BigInt diff = b - a;
    if (diff.sign) {
        return true;
    } else {
        return false;
    }
}

bool operator <=(const BigInt &a, const BigInt &b) {
    if (a == b) {
        return true;
    }
    BigInt diff = b - a;
    if (diff.sign) {
        return true;
    } else {
        return false;
    }
}
