#include "my_vector.h"
#include <iostream>
typedef unsigned int uint;


void my_vector::reserve(size_t capacity) {
    if (capacity >= get_capacity() && capacity > SMALL_SIZE) {
        // take copy of existing + zeroes to end
        uint* copy = new uint[capacity];
        std::memcpy(copy, actual_data, _size * sizeof(uint));
        std::memset(copy + _size, 0, (capacity - _size) * sizeof(uint));
        if (is_big) {
            data.big.pointer.reset(copy);
            data.big.capacity = capacity;
            // reset pointer to copy, change capacity
        } else {
            new (&data.big) big_data(copy, capacity);
            // make placement new big_data with copy and new capacity
        }
        is_big = true;
        actual_data = data.big.pointer.get();
    }
}

my_vector::my_vector() : _size(0), is_big(false), actual_data(data.small) {
    std::memset(actual_data, 0, SMALL_SIZE * sizeof(uint));
}

my_vector::my_vector(size_t size) : my_vector() {
    reserve(size);
    _size = size;
}

my_vector::my_vector(my_vector const &other) : my_vector() {
    if (other.is_big) {
        is_big = true;
        new(&data.big) big_data(other.data.big);
        actual_data = data.big.pointer.get();
    } else {
        is_big = false;
        std::memcpy(data.small, other.data.small, SMALL_SIZE * sizeof(uint));
        actual_data = data.small;
    }
    _size = other._size;
}

my_vector::~my_vector() {
    if (is_big) {

        int a = 2;
        data.big.~big_data();
    }
}

void my_vector::resize(size_t new_size) {
    reserve(new_size);
    _size = new_size;
}

size_t my_vector::size() const {
    return _size;
}

bool my_vector::empty() {
    return _size == 0;
}

uint &my_vector::operator[](size_t index) {
    return actual_data[index];
}

uint const &my_vector::operator[](size_t index) const {
    return actual_data[index];
}

void my_vector::pop_back() {
    _size --;
}

void my_vector::push_back(uint value) {
    if (get_capacity() < _size + 1) {
        reserve((get_capacity() * 3) >> 1);
    }
    actual_data[_size ++] = value;
}

uint my_vector::back() const {
    return actual_data[_size - 1];
}

void my_vector::reverse() {
    if (_size == 1) return;
    reserve(get_capacity());
    for (size_t i = 0; i < (_size >> 1); i++) {
        std::swap(actual_data[i], actual_data[_size - i - 1]);
    }
}

bool operator==(const my_vector &a, const my_vector &b) {
    return (a._size == b._size) && (std::memcmp(a.actual_data, b.actual_data, a._size * sizeof(uint)) == 0);
}

void my_vector::swap_diff(my_vector::any_data &big, my_vector::any_data &small) {
    uint temp[SMALL_SIZE];
    std::memcpy(temp, small.small, SMALL_SIZE * sizeof(uint));
    new(&small.big) big_data(big.big);
    big.big.~big_data();

    std::memcpy(big.small, temp, SMALL_SIZE * sizeof(uint));
}

void my_vector::swap(my_vector &other) {
    if (is_big && other.is_big) {
        std::swap(data.big, other.data.big);
        actual_data = data.big.pointer.get();
        other.actual_data = other.data.big.pointer.get();
    } else if (!is_big && !other.is_big) {
        for (size_t i = 0; i < SMALL_SIZE; i++) {
            std::swap(data.small[i], other.data.small[i]);
        }
    } else if (is_big) {
        swap_diff(data, other.data);
        actual_data = data.small;
        other.actual_data = other.data.big.pointer.get();
    } else {
        swap_diff(other.data, data);
        actual_data = data.big.pointer.get();
        other.actual_data = other.data.small;
    }
    std::swap(is_big, other.is_big);
    std::swap(_size, other._size);
}

my_vector& my_vector::operator=(my_vector const &other) {
    my_vector temp(other);
    swap(temp);
    return *this;
}










