#ifndef MY_VECTOR
#define MY_VECTOR

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <variant>

struct my_vector {
   public:
    typedef unsigned int uint;

    my_vector();
    ~my_vector();

    explicit my_vector(size_t size);
    my_vector(my_vector const& other);

    void reserve(size_t capacity);
    void resize(size_t new_size);
    size_t size() const;
    bool empty();

    uint& operator[](size_t index);
    uint const& operator[](size_t index) const;

    my_vector& operator=(my_vector const& other);

    void pop_back();
    void push_back(uint value);
    uint back() const;

    uint* begin();
    uint* end();

    friend bool operator==(const my_vector& a, const my_vector& b);

    void swap(my_vector& other) noexcept;

   private:
    static const size_t SMALL_SIZE = 4;

    size_t _size;
    bool is_big;

    struct big_data {
        std::shared_ptr<uint[]> pointer;
        size_t capacity;

        big_data(uint* p, size_t cap) : pointer(p), capacity(cap) {}

        big_data(const big_data& other)
            : pointer(other.pointer), capacity(other.capacity) {}
    };

    union any_data {
        big_data big;
        uint small[SMALL_SIZE]{};

        any_data() {}
        ~any_data() {}
    } data;

    uint* actual_data;

    size_t get_capacity() { return is_big ? data.big.capacity : SMALL_SIZE; }
    void swap_diff(any_data& big, any_data& small) noexcept;

    void prepare();
};

#endif
