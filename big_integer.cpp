#include "big_integer.h"
#include <algorithm>
#include <stdexcept>
#include <string>
using namespace std;

typedef unsigned int uint;
typedef unsigned long long ull;

const uint BASE = 1000000000;
const uint MAX_D = UINT32_MAX;
const ull TRUE_BASE = (ull)UINT32_MAX + 1;
const uint DIGIT_CAPACITY = 32;

//      details

uint decimal_power(uint p) {
    uint result = 1;
    uint val = 10;
    while (p > 0) {
        if (p % 2 == 1) result *= val;
        val *= val;
        p /= 2;
    }
    return result;
}

big_integer big_integer::abs() const { return big_integer(false, number); }

size_t big_integer::length() const { return this->number.size(); }

size_t big_integer::find_not_zero() const {
    size_t i = 0;
    while (i < number.size() && number[i] == 0) i++;
    return i;
}

uint big_integer::get_complement_digit(size_t index, size_t fnz) const {
    if (index < number.size()) {
        if (sign) {
            return (index > fnz) ? ~number[index] : -number[index];
        } else {
            return number[index];
        }
    } else if (sign) {
        return MAX_D;
    } else {
        return 0;
    }
}

bool big_integer::is_neg() const { return this->sign; }

void big_integer::make_fit() {
    while (number.size() > 0 && ((!sign && number.back() == 0) ||
                                 (sign && number.back() == MAX_D))) {
        number.pop_back();
    }
    if (number.empty()) sign = false;
}

void big_integer::strip_zeroes() {
    while (!number.empty() && number.back() == 0) {
        number.pop_back();
    }
    if (number.empty()) {
        sign = false;
    }
}

//      constructors

big_integer::big_integer() : sign(false) {}

big_integer::big_integer(int a) : number((a != 0) ? 1 : 0), sign(a < 0) {
    if (a != 0) {
        if (a == INT32_MIN) {
            number[0] = (uint)a;
        } else {
            number[0] = (a < 0) ? -a : a;
        }
    }
}

big_integer::big_integer(uint a) : number(1), sign(false) {
    number[0] = a;
    strip_zeroes();
}

big_integer::big_integer(bool s, my_vector n) : number(n), sign(s) {
    strip_zeroes();
}

big_integer::big_integer(long long a) : number(2), sign(a < 0) {
    a = (a < 0) ? -a : a;
    if (a > MAX_D) {
        number[1] = a / TRUE_BASE;
        number[0] = a - number[1] * TRUE_BASE;
    } else {
        number[0] = a;
    }
    strip_zeroes();
}

big_integer::big_integer(big_integer const &other)
    : number(other.number), sign(other.sign) {
    strip_zeroes();
}

big_integer::big_integer(std::string const &str) {
    big_integer res;
    size_t is_negative = 0;
    if (str[is_negative] == '-') {
        is_negative++;
    }
    for (size_t i = is_negative; i + 9 <= str.length(); i += 9) {
        res = res * BASE + stoi(str.substr(i, 9));
    }
    uint mod = (str.length() - is_negative) % 9;
    if (mod > 0) {
        res = res * decimal_power(mod) + stoi(str.substr(str.length() - mod));
    }
    *this = (is_negative != 0) ? -res : res;
}

//      assignment

big_integer &big_integer::operator=(big_integer const &other) {
    this->number = other.number;
    this->sign = other.sign;
    make_fit();
    return *this;
}

//      bitwise operations

typedef uint (*bitwise_func_t)(uint, uint);

big_integer &big_integer::operator&=(big_integer const &rhs) {
    return *this = *this & rhs;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    return *this = *this | rhs;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    return *this = *this ^ rhs;
}

uint do_or(uint a, uint b) { return a | b; }
uint do_xor(uint a, uint b) { return a ^ b; }
uint do_and(uint a, uint b) { return a & b; }

big_integer do_bitwise(big_integer a, big_integer const &b,
                       bitwise_func_t func) {
    size_t len = max(a.length(), b.length());
    my_vector temporary(len);
    size_t a_fnz = a.find_not_zero();
    size_t b_fnz = b.find_not_zero();
    for (size_t i = 0; i < len; i++) {
        temporary[i] = (*func)(a.get_complement_digit(i, a_fnz),
                               b.get_complement_digit(i, b_fnz));
    }
    bool nsign = (*func)(a.is_neg(), b.is_neg());
    if (!nsign) {
        return big_integer(nsign, temporary);
    } else {
        for (size_t i = 0; i < len; i++) {
            temporary[i] = ~temporary[i];
        }
        big_integer res(nsign, temporary);
        return res - 1;
    }
}

big_integer operator&(big_integer a, big_integer const &b) {
    return do_bitwise(a, b, do_and);
}

big_integer operator|(big_integer a, big_integer const &b) {
    return do_bitwise(a, b, do_or);
}

big_integer operator^(big_integer a, big_integer const &b) {
    return do_bitwise(a, b, do_xor);
}

//      unary operations

big_integer big_integer::operator+() const {
    big_integer result(*this);
    result.sign = this->sign;
    result.make_fit();
    return result;
}

big_integer big_integer::operator-() const {
    big_integer result(*this);
    result.sign = !this->sign;
    result.make_fit();
    return result;
}

big_integer big_integer::operator~() const { return -*this - 1; }

big_integer &big_integer::operator++() {
    *this += 1;
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer temporary(*this);
    ++(*this);
    return temporary;
}

big_integer &big_integer::operator--() {
    *this -= 1;
    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer temporary(*this);
    --(*this);
    return temporary;
}

//      comparison

bool operator==(big_integer const &a, big_integer const &b) {
    return (a.sign == b.sign) && (a.number == b.number);
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return a.sign;
    }
    if (a.number.size() != b.number.size()) {
        return (a.number.size() < b.number.size()) ^ a.sign;
    }
    for (size_t i = a.number.size(); i > 0; i--) {
        if (a.number[i - 1] != b.number[i - 1]) {
            return (a.number[i - 1] < b.number[i - 1]) ^ a.sign;
        }
    }
    return false;
}

bool operator>(big_integer const &a, big_integer const &b) { return b < a; }

bool operator<=(big_integer const &a, big_integer const &b) { return !(b < a); }

bool operator>=(big_integer const &a, big_integer const &b) { return !(a < b); }

//      string converting

string to_string(big_integer const &a) {
    string result;
    if (a.length() == 0) {
        return "0";
    }
    big_integer b(a.abs());
    while (b.length() != 0) {
        big_integer mod = b % BASE;
        uint temporary = 0;
        if (mod.length() != 0) {
            temporary = mod.number[0];
        }
        for (size_t i = 0; i < 9; i++) {
            result.push_back('0' + temporary % 10);
            temporary /= 10;
        }
        b /= BASE;
    }
    while (!result.empty() && result.back() == '0') {
        result.pop_back();
    }
    if (a.sign) {
        result.push_back('-');
    }
    reverse(result.begin(), result.end());
    return result;
}

ostream &operator<<(std::ostream &s, big_integer const &a) {
    return (s << to_string(a));
}

//      addition

big_integer &big_integer::operator+=(big_integer const &rhs) {
    return *this = *this + rhs;
}

big_integer operator+(big_integer a, big_integer const &b) {
    if (a.is_neg() == b.is_neg()) {
        ull interim_res;
        ull carry = 0;
        size_t max_len = max(a.length(), b.length());
        my_vector result(max_len);
        for (size_t i = 0; i < max_len; i++) {
            interim_res = (ull)((i < a.length()) ? a.number[i] : 0) +
                          (ull)((i < b.length()) ? b.number[i] : 0) + carry;
            carry = interim_res / TRUE_BASE;
            interim_res -= TRUE_BASE * carry;
            result[i] = interim_res;
        }
        if (carry > 0) {
            result.push_back(carry);
        }
        return big_integer(a.is_neg(), result);
    } else if (a.is_neg()) {
        return b - (-a);
    } else {
        return a - (-b);
    }
}

//      subtraction

big_integer &big_integer::operator-=(big_integer const &rhs) {
    return *this = *this - rhs;
}

big_integer do_sub_saving_sign(big_integer a, big_integer const &b) {
    long long interim_res;
    ull carry = 0;
    size_t max_len = max(a.length(), b.length());
    my_vector result(max_len);
    for (size_t i = 0; i < max_len; i++) {
        interim_res = (ull)((i < a.length()) ? a.number[i] : 0) -
                      (ull)((i < b.length()) ? b.number[i] : 0) - carry;
        if (interim_res < 0) {
            interim_res += TRUE_BASE;
            carry = 1;
        } else {
            carry = 0;
        }
        result[i] = interim_res;
    }
    return big_integer(a.is_neg(), result);
}

big_integer operator-(big_integer a, big_integer const &b) {
    if (a.is_neg() == b.is_neg()) {
        if (a.is_neg()) {
            if (b < a) {
                return -(b - a);
            } else {
                return do_sub_saving_sign(a, b);
            }
        } else {
            if (a < b) {
                return -(b - a);
            } else {
                return do_sub_saving_sign(a, b);
            }
        }
    } else {
        if (a.is_neg())
            return -((-a) + b);
        else
            return a + (-b);
    }
}

//      multiplication

big_integer &big_integer::operator*=(big_integer const &rhs) {
    return *this = *this * rhs;
}

void mul_long_short(my_vector &res, my_vector const a, const uint b) {
    size_t len = a.size();
    res.resize(len + 1);
    ull carry = 0;
    ull interim_res = 0;
    for (size_t i = 0; i < len; i++) {
        interim_res = ull(a[i]) * b + carry;
        carry = interim_res / TRUE_BASE;
        res[i] = interim_res - carry * TRUE_BASE;
    }
    if (carry)
        res[len] = carry;
    else
        res.pop_back();
}

void mul_long_long(my_vector &res, my_vector a, my_vector b) {
    for (size_t i = 0; i < a.size(); i++) {
        for (ull j = 0, carry = 0; j < b.size() || carry; j++) {
            ull interim_res =
                res[i + j] + a[i] * ull(j < b.size() ? b[j] : 0) + carry;
            carry = interim_res / TRUE_BASE;
            res[i + j] = interim_res - carry * TRUE_BASE;
        }
    }
}

big_integer operator*(big_integer a, big_integer const &b) {
    if (a.length() == 0 || b.length() == 0) {
        return 0;
    }
    if (a.length() > b.length()) {
        return b * a;
    }
    my_vector temporary(a.length() + b.length() + 1);
    if (a.length() == 1) {
        mul_long_short(temporary, b.abs().number, a.abs().number[0]);
    } else {
        mul_long_long(temporary, a.abs().number, b.abs().number);
    }
    while (temporary.size() > 0 && temporary.back() == 0) temporary.pop_back();
    return big_integer(a.sign ^ b.sign, temporary);
}

//      division

big_integer &big_integer::operator/=(big_integer const &rhs) {
    return *this = *this / rhs;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    return *this = *this % rhs;
}

void div_long_short(my_vector &result, my_vector a, uint b) {
    size_t m = a.size();
    result.resize(m + 1);
    ull carry = 0;
    for (long long i = m - 1; i >= 0; i--) {
        ull temp = carry * TRUE_BASE + a[i];
        result[i] = temp / b;
        carry = temp % b;
    }
    while (!result.empty() && result.back() == 0) {
        result.pop_back();
    }
}

uint trial(big_integer const &r, big_integer const &d, uint k, uint m) {
    uint km = k + m;
    if (r == 0) return 0;
    ull r1 = (ull(r.number[km]) << (DIGIT_CAPACITY)) + (ull(r.number[km - 1]));
    ull d1 = ull(d.number[m - 1]);
    ull left = r1 / d1;
    return min(left, ull(MAX_D));
}

bool smaller(big_integer const &r, big_integer const &d, uint k, uint m) {
    size_t i = m, j = 0;
    while (i != j) {
        if (r.number[i + k] != d.number[i])
            j = i;
        else
            --i;
    }
    return r.number[i + k] < d.number[i];
}

big_integer divide(big_integer const &x, big_integer const &y) {
    big_integer q, r, d;
    size_t n = x.length();
    size_t m = y.length();
    uint f = TRUE_BASE / (ull(y.number.back()) + 1);
    if (f != 0) {
        r = x * f;
    } else {
        r = x << (DIGIT_CAPACITY);
    }
    d = y * f;
    if (f == 0) {
        d = y << (DIGIT_CAPACITY);
    }
    q.number.resize(n - m + 2);
    r.number.push_back(0);
    for (size_t k = n - m; k < n - m + 1; k--) {
        uint qt = trial(r, d, k, m);
        big_integer dq = d * qt;
        if (qt == 0) continue;
        dq.number.push_back(0);
        while (smaller(r, dq, k, m)) {
            qt--;
            dq -= d;
        }
        q.number[k] = qt;
        r -= dq << (DIGIT_CAPACITY * k);
    }
    q.strip_zeroes();
    return q;
}

big_integer operator/(big_integer a, big_integer const &b) {
    size_t m = b.length();
    if (m == 0) throw(runtime_error("division by zero"));
    if (m == 1) {
        my_vector quot;
        div_long_short(quot, a.number, b.number[0]);
        return big_integer(a.sign ^ b.sign, quot);
    } else {
        size_t n = a.length();
        if (m > n) {
            return 0;
        } else {
            if (a.sign ^ b.sign) {
                return -divide(a.abs(), b.abs());
            } else {
                return divide(a.abs(), b.abs());
            }
        }
    }
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a - a / b * b;
}

//     shifts

big_integer &big_integer::operator<<=(int rhs) { return *this = *this << rhs; }

big_integer &big_integer::operator>>=(int rhs) { return *this = *this >> rhs; }

big_integer operator<<(big_integer a, int b) {
    if (a == 0 || b == 0) {
        return a;
    }
    uint zero_digits = b / DIGIT_CAPACITY;
    uint shift_digit = b % DIGIT_CAPACITY;
    big_integer result(a);
    uint carry = 0;
    if (shift_digit != 0) {
        for (size_t i = 0; i < result.length(); i++) {
            uint interim = (result.number[i] << shift_digit) | carry;
            carry = result.number[i] >> (DIGIT_CAPACITY - shift_digit);
            result.number[i] = interim;
        }
    }
    // FAAAASTEER
    result.number.insert_zeroes_in_begin(zero_digits);

    return result;
}

big_integer operator>>(big_integer a, int b) {
    if (a.sign) {
        a -= 1;
    }
    uint deleted_digits = b / DIGIT_CAPACITY;
    uint shift_digit = b % DIGIT_CAPACITY;
    uint carry = 0;
    my_vector res;
    for (size_t i = a.length(); i > deleted_digits; i--) {
        uint interim = (a.number[i - 1] >> shift_digit) &
                       (uint(1 << (DIGIT_CAPACITY - shift_digit)) - 1);
        interim |= carry;
        carry = (a.number[i - 1] & (uint(1 << shift_digit) - 1))
                << (DIGIT_CAPACITY - shift_digit);
        res.push_back(interim);
    }
    reverse(res.begin(), res.end());
    big_integer result(a.sign, res);
    if (a.sign) {
        a += 1;
        return result -= 1;
    }
    return result;
}
