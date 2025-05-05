#ifndef MY_BITSET
#define MY_BITSET

#include <string>

typedef unsigned long long bits_partition_t;
const bits_partition_t MIN_BITS_PARTITION_VALUE(0);
const bits_partition_t MAX_BITS_PARTITION_VALUE(~MIN_BITS_PARTITION_VALUE);

enum class base
{
    HEX,
    BIN
};

class my_bitset
{
private:
    bits_partition_t *bits;
    bits_partition_t *mask;
    unsigned long long array_size;
    unsigned long long bits_size;

    bits_partition_t char_to_hex(const char &ch);
    void reverse(bits_partition_t &val);

public:
    my_bitset();
    my_bitset(const my_bitset &obj);
    my_bitset(const unsigned long long bits_size, const unsigned long long value = 0);
    my_bitset(const std::string str, const base b);
    ~my_bitset();

    unsigned long long get_size();

    bool at(const unsigned long long idx);
    void set(const unsigned long long idx);
    void reset();
    unsigned long long count();

    void resize(unsigned long long size, unsigned long long value = 0);
    void reverse_bits(unsigned long long window_size);

    std::string to_string();
    unsigned long long to_ull();

    my_bitset operator^(const my_bitset &obj);
    my_bitset &operator^=(const my_bitset &obj);
    my_bitset &operator>>=(const unsigned long long shift);
    my_bitset &operator<<=(const unsigned long long shift);
    my_bitset &operator=(const my_bitset &obj);
    my_bitset &operator=(unsigned long long value);
    my_bitset &operator&=(const my_bitset &obj);
    void operator++();

    bool operator<(const my_bitset &obj);
    bool operator<(const my_bitset &obj) const;
};

#endif // !MY_BITSET