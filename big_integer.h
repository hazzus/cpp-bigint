#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
//#include <vector>
#include "vector/my_vector.h"

typedef unsigned int uint;

struct big_integer {
    big_integer(bool s, my_vector n);
    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
    big_integer(long long a);
    big_integer(uint a);
    explicit big_integer(std::string const& str);
    ~big_integer() = default;

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

    friend big_integer operator&(big_integer a, big_integer const& b);
    friend big_integer operator|(big_integer a, big_integer const& b);
    friend big_integer operator^(big_integer a, big_integer const& b);

    friend big_integer operator*(big_integer a, big_integer const& b);
    friend big_integer operator/(big_integer a, big_integer const& b);
    friend big_integer operator+(big_integer a, big_integer const& b);
    friend big_integer operator-(big_integer a, big_integer const& b);

    friend std::string to_string(big_integer const& a);
    friend big_integer do_sub_saving_sign(big_integer a, big_integer const& b);
    friend big_integer div_long_short(big_integer const& a, uint x);
    friend big_integer division(big_integer const& a, big_integer const& b);
    friend big_integer divide(big_integer const& x, big_integer const& y);
    friend big_integer do_bitwise(big_integer a, big_integer const& b,
                                  uint (*func)(uint, uint));

    friend big_integer operator<<(big_integer a, int b);
    friend big_integer operator>>(big_integer a, int b);

    friend uint trial(big_integer const& r, big_integer const& d, uint k,
                      uint m);
    friend bool smaller(big_integer const& r, big_integer const& d, uint k,
                        uint m);
    friend void differ(big_integer& r, big_integer dq, uint k, uint m);

    size_t length() const;
    uint get_complement_digit(size_t index, size_t fnz) const;
    bool is_neg() const;
    big_integer abs() const;

   private:
    size_t find_not_zero() const;
    void strip_zeroes();
    void make_fit();
    my_vector number;
    bool sign;
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);
big_integer do_sub_saving_sign(big_integer a, big_integer const& b);
big_integer div_long_short(big_integer const& a, uint x);
big_integer division(big_integer const& a, big_integer const& b);
big_integer divide(big_integer const& x, big_integer const& y);
big_integer do_bitwise(big_integer a, big_integer const& b,
                       uint (*func)(uint, uint));

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);  //и вот эти
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);

#endif  // BIG_INTEGER_H
