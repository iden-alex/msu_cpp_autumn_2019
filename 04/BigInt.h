#ifndef BIGINT_H
#define BIGINT_H

class BigInt {
    size_t len;
    bool sign;
    char *ptr;
public:
    BigInt(long long = 0);
    BigInt(const BigInt &);
    ~BigInt();
    BigInt& operator=(long long);
    BigInt& operator=(const BigInt&);
    BigInt operator -() const;
    BigInt Add_pos_arg(const BigInt &) const;
    BigInt Add_neg_arg(const BigInt &) const;
    bool Is_mod_greater(const BigInt &b) const;
    
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
#endif
