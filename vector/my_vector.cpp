#include "my_vector.h"
typedef unsigned int uint;

uint *take_copy(uint *data, size_t cap, size_t size) {
    uint *copy = new uint[cap];
    std::memcpy(copy, data, size * sizeof(uint));
    return copy;
}

void my_vector::reserve(size_t capacity) {
    if (capacity > get_capacity() && capacity > SMALL_SIZE) {
        capacity = std::max(capacity, get_capacity() * GROWTH);
        if (is_big) {
            data.big.pointer.reset(take_copy(actual_data, capacity, _size));
            data.big.capacity = capacity;
        } else {
            new (&data.big)
                big_data(take_copy(actual_data, capacity, _size), capacity);
            is_big = true;
        }
        actual_data = data.big.pointer.get();
    }
}

void my_vector::prepare() {
    if (is_big && !data.big.pointer.unique()) {
        data.big.pointer.reset(
            take_copy(actual_data, data.big.capacity * GROWTH, _size));
        data.big.capacity *= GROWTH;
        actual_data = data.big.pointer.get();
    }
}

my_vector::my_vector() : _size(0), is_big(false), actual_data(data.small) {}

my_vector::my_vector(size_t size) : my_vector() {
    reserve(size);
    std::memset(actual_data, 0, size * sizeof(uint));
    _size = size;
}

my_vector::my_vector(my_vector const &other) : my_vector() {
    if (other.is_big) {
        is_big = true;
        new (&data.big) big_data(other.data.big);
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
        data.big.~big_data();
    }
}

void my_vector::resize(size_t new_size) {
    reserve(new_size);
    if (new_size > _size)
        std::memset(actual_data + _size, 0, (new_size - _size) * sizeof(uint));
    _size = new_size;
}

size_t my_vector::size() const { return _size; }

bool my_vector::empty() { return _size == 0; }

void my_vector::insert_zeroes_in_begin(size_t count) {
    size_t n_capacity = std::max(get_capacity() * GROWTH, _size + count);
    uint *copy = new uint[n_capacity];
    std::memcpy(copy + count, actual_data, _size * sizeof(uint));
    std::memset(copy, 0, count * sizeof(uint));
    if (n_capacity > SMALL_SIZE) {
        if (is_big) {
            data.big.pointer.reset(copy);
            data.big.capacity = n_capacity;
        } else {
            new (&data.big) big_data(copy, n_capacity);
            is_big = true;
        }
        actual_data = data.big.pointer.get();
    } else {
        std::memcpy(data.small, copy, SMALL_SIZE * sizeof(uint));
    }
    _size += count;
}

uint &my_vector::operator[](size_t index) {
    prepare();
    return actual_data[index];
}

uint const &my_vector::operator[](size_t index) const {
    return actual_data[index];
}

void my_vector::pop_back() { _size--; }

void my_vector::push_back(uint value) {
    if (get_capacity() < _size + 1) {
        reserve(get_capacity() * GROWTH);
    } else {
        prepare();
    }
    actual_data[_size++] = value;
}

uint my_vector::back() const { return actual_data[_size - 1]; }

bool operator==(const my_vector &a, const my_vector &b) {
    return (a._size == b._size) && (std::memcmp(a.actual_data, b.actual_data,
                                                a._size * sizeof(uint)) == 0);
}

void my_vector::swap_diff(my_vector::any_data &big,
                          my_vector::any_data &small) noexcept {
    uint temp[SMALL_SIZE];
    std::memcpy(temp, small.small, SMALL_SIZE * sizeof(uint));
    new (&small.big) big_data(big.big);
    big.big.~big_data();
    std::memcpy(big.small, temp, SMALL_SIZE * sizeof(uint));
}

void my_vector::swap(my_vector &other) noexcept {
    if (is_big && other.is_big) {
        std::swap(data.big, other.data.big);
        actual_data = data.big.pointer.get();
        other.actual_data = other.data.big.pointer.get();
    } else if (!is_big && !other.is_big) {
        std::swap(data.small, other.data.small);
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

my_vector &my_vector::operator=(my_vector const &other) {
    my_vector temp(other);
    swap(temp);
    return *this;
}

uint *my_vector::begin() { return actual_data; }

uint *my_vector::end() { return actual_data + _size; }
