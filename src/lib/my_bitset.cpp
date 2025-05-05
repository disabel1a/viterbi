#include "../include/my_bitset.h"

#include <functional>
#include <stdexcept>

bits_partition_t my_bitset::char_to_hex(const char &ch)
{
    bits_partition_t result(0);

    if (ch < 48 || ch > 70)
    {
        return result;
    }

    if (ch > 57)
    {
        if (ch < 65)
        {
            return result;
        }
        else
        {
            result = (bits_partition_t)(ch - (48 + 7));
        }
    }
    else
    {
        result = (bits_partition_t)(ch - 48);
    }

    reverse(result);

    return result;
}

void my_bitset::reverse(bits_partition_t &val)
{
    bits_partition_t tmp(0);

    for (int i(0); i < 4; ++i)
    {
        tmp <<= 1;
        tmp |= (val & 1);
        val >>= 1;
    }

    val = tmp;
}

my_bitset::my_bitset()
{
    bits = new bits_partition_t[1];
    mask = new bits_partition_t[1];
    array_size = 1;
    bits_size = sizeof(bits_partition_t) << 3;

    bits[0] = 0;
    mask[0] = MAX_BITS_PARTITION_VALUE;
}

my_bitset::my_bitset(const my_bitset &obj)
{
    this->bits_size = obj.bits_size;
    this->array_size = obj.array_size;

    this->bits = new bits_partition_t[this->array_size];
    this->mask = new bits_partition_t[this->array_size];

    for (unsigned long long iter(0); iter < this->array_size; ++iter)
    {
        this->bits[iter] = obj.bits[iter];
        this->mask[iter] = obj.mask[iter];
    }
}

my_bitset::my_bitset(const unsigned long long bits_size, const unsigned long long value)
{
    if (bits_size == 0)
    {
        throw std::invalid_argument("Bitset cannot have size equal to 0");
    }

    this->bits_size = bits_size;
    array_size = (bits_size / (sizeof(bits_partition_t) << 3)) + 1;
    
    bits = new bits_partition_t[array_size];
    mask = new bits_partition_t[array_size];

    bits[0] = value;
    mask[array_size - 1] = ~(MAX_BITS_PARTITION_VALUE << (bits_size - ((sizeof(bits_partition_t) << 3) * (array_size - 1))));

    if (array_size == 1)
    {
        bits[0] &= mask[0];
    }

    for (unsigned long long i = 1; i < array_size; ++i)
    {
        bits[i] = 0;
        mask[(array_size - 1) - i] = MAX_BITS_PARTITION_VALUE;
    }
}

my_bitset::my_bitset(const std::string str, const base b)
{
    unsigned long long current_part(0);
    unsigned long long partition(0);

    std::function<bits_partition_t(const char &, unsigned long long)> func;

    switch(b)
    {
        case base::BIN:
            partition = 1;
            func = [this](const char &ch,unsigned long long current_part)
            {
                return (ch == 49) ? bits_partition_t(1) << current_part : bits_partition_t(0);
            };
            break;

        default:
            partition = 4;
            func = [this](const char &ch, unsigned long long current_part)
            {
                return char_to_hex(ch) << current_part;
            };
        break;
    }

    bits_size = partition * str.size();
    array_size = bits_size / (sizeof(bits_partition_t) << 3) + 1;

    bits_partition_t current_symbol(0);
    unsigned long long array_index(0);

    bits = new bits_partition_t[array_size];
    mask = new bits_partition_t[array_size];

    for (unsigned long long iter(0); iter < str.size(); ++iter)
    {
        current_symbol ^= func(str[iter], current_part);
        current_part += partition;

        if (current_part == (sizeof(bits_partition_t) << 3))
        {
            mask[array_index] = MAX_BITS_PARTITION_VALUE;
            bits[array_index] = current_symbol;
            current_symbol = 0;
            ++array_index;
        }
        else if (iter == (str.size() - 1))
        {
            mask[array_index] = MAX_BITS_PARTITION_VALUE >> (array_size * (sizeof(bits_partition_t) << 3) - bits_size);
            bits[array_index] = current_symbol;
        }
    }
}

my_bitset::~my_bitset()
{
    delete [] bits;
    delete [] mask;
}

unsigned long long my_bitset::get_size()
{
    return bits_size;
}

bool my_bitset::at(const unsigned long long idx)
{
    if (idx >= this->bits_size)
    {
        throw std::out_of_range("Unavaliable bit index in function 'at'");
    }

    unsigned long long pos_in_arr = idx / (sizeof(bits_partition_t) << 3);
    unsigned long long local_idx = idx - (pos_in_arr * sizeof(bits_partition_t));

    return (this->bits[pos_in_arr] >> local_idx) & 1;
}

void my_bitset::set(const unsigned long long idx)
{
    if (idx >= this->bits_size)
    {
        throw std::out_of_range("Unavaliable bit index in function 'at'");
    }

    unsigned long long pos_in_arr = idx / (sizeof(bits_partition_t) << 3);
    unsigned long long local_idx = idx - (pos_in_arr * sizeof(bits_partition_t));

    this->bits[pos_in_arr] |= (bits_partition_t(1) << local_idx);
}

void my_bitset::reset()
{
    for (unsigned long long iter(0); iter < this->array_size; ++iter)
    {
        bits[iter] = bits_partition_t(0);
    }
}

unsigned long long my_bitset::count()
{
    unsigned long long counter(0);
    bits_partition_t tmp(0);

    for (unsigned long long iter(0); iter < this->array_size; ++iter)
    {
        tmp = bits[iter];

        for (unsigned long long bit_idx(0); bit_idx < (sizeof(bits_partition_t) << 3); ++bit_idx)
        {
            counter += (tmp & 1);
            tmp >>= 1;
        }
    }

    return counter;
}

void my_bitset::resize(unsigned long long size, unsigned long long value)
{
    if (bits_size == 0)
    {
        throw std::invalid_argument("Bitset cannot have size equal to 0");
    }

    delete [] bits;
    delete [] mask;

    this->bits_size = bits_size;
    array_size = (bits_size / (sizeof(bits_partition_t) << 3)) + 1;
    
    bits = new bits_partition_t[array_size];
    mask = new bits_partition_t[array_size];

    bits[0] = value;
    mask[array_size - 1] = ~(MAX_BITS_PARTITION_VALUE << (bits_size - ((sizeof(bits_partition_t) << 3) * (array_size - 1))));

    if (array_size == 1)
    {
        bits[0] &= mask[0];
    }

    for (unsigned long long i = 1; i < array_size; ++i)
    {
        bits[i] = 0;
        mask[(array_size - 1) - i] = MAX_BITS_PARTITION_VALUE;
    }
}

void my_bitset::reverse_bits(unsigned long long widnow_size)
{
    my_bitset window(widnow_size, 0);
    my_bitset copy(this->bits_size, 0);

    for (uint64_t iter(0); iter < (bits_size / widnow_size); ++iter)
    {
        copy <<= widnow_size;
        window = *this;
        copy ^= window;
        *this >>= widnow_size;
    }

    *this = copy;
}

std::string my_bitset::to_string()
{
    std::string str;
    str = std::to_string(bits_size) + "'b";

    unsigned long long bits_counter(0);

    for (unsigned long long iter(0); iter < array_size; ++iter)
    {
        for (unsigned long long bit_index(0); bit_index < (sizeof(bits_partition_t) << 3); ++bit_index)
        {
            if (bits_counter == bits_size)
            {
                return str;
            }

            str += std::to_string((bits[iter] >> bit_index) & 1);
            ++bits_counter;
        }
    }

    return str;
}

unsigned long long my_bitset::to_ull()
{
    unsigned long long partition = sizeof(unsigned long long) / sizeof(bits_partition_t);

    unsigned long long value = 0;

    for (unsigned long long iter(0); iter < partition && iter < array_size; ++iter)
    {
        unsigned long long tmp = bits[iter];
        tmp <<= iter * (sizeof(bits_partition_t) << 3);
        value ^= tmp;
    }

    return value;
}

my_bitset my_bitset::operator^(const my_bitset &obj)
{
    my_bitset copy(*this);
    copy ^= obj;
    return copy;
}

my_bitset &my_bitset::operator^=(const my_bitset &obj)
{
    unsigned long long last_idx = (this->bits_size > obj.bits_size) ? obj.array_size : this->array_size;

    for (unsigned long long iter(0); iter < last_idx; ++iter)
    {
        this->bits[iter] ^= obj.bits[iter];
    }

    this->bits[array_size - 1] &= this->mask[array_size - 1];

    return *this;
}

my_bitset &my_bitset::operator>>=(const unsigned long long shift)
{
    bits_partition_t move_mask = MAX_BITS_PARTITION_VALUE >> shift;
    bits_partition_t move_part(0);

    bits[0] >>= shift;

    for (unsigned long long iter(1); iter < array_size; ++iter)
    {
        move_part = bits[iter] & move_mask;
        bits[iter] >>= shift;
        bits[iter - 1] ^= (move_part << ((sizeof(bits_partition_t) << 3) - shift));
    }

    return *this;
}

my_bitset &my_bitset::operator<<=(const unsigned long long shift)
{
    bits_partition_t move_mask = MAX_BITS_PARTITION_VALUE << shift;
    bits_partition_t move_part(0);

    bits[array_size - 1] <<= shift;
    bits[array_size - 1] &= mask[array_size - 1];

    if (array_size == 1)
    {
        return *this;
    }

    for (unsigned long long iter(array_size - 2); iter >= 0; --iter)
    {
        move_part = bits[iter] & move_mask;
        bits[iter] <<= shift;
        bits[iter + 1] ^= (move_part >> ((sizeof(bits_partition_t) << 3) - shift));

        if (iter == array_size - 2)
        {
            bits[array_size - 1] &= mask[array_size - 1];
        }

        if (iter == 0)
        {
            break;
        }
    }

    return *this;
}

my_bitset &my_bitset::operator=(const my_bitset &obj)
{
    unsigned long long last_idx = (this->bits_size > obj.bits_size) ? obj.array_size : this->array_size;

    for (unsigned long long iter(0); iter < last_idx; ++iter)
    {
        this->bits[iter] = obj.bits[iter];
    }

    if (last_idx == obj.array_size)
    {
        for (unsigned long long iter(last_idx); iter < this->array_size; ++iter)
        {
            this->bits[iter] = bits_partition_t(0);
        }
    }

    this->bits[last_idx - 1] &= this->mask[last_idx - 1];

    return *this;
}

my_bitset &my_bitset::operator=(unsigned long long value)
{
    this->reset();

    unsigned long long value_partition = sizeof(value) / sizeof(bits_partition_t);

    for (unsigned long long iter(0); iter < value_partition; ++iter)
    {
        bits[iter] = value;
        value >>= (sizeof(bits_partition_t) >> 3);
    }

    bits[array_size - 1] &= mask[array_size - 1];

    return *this;
}

my_bitset &my_bitset::operator&=(const my_bitset &obj)
{
    unsigned long long last_idx = (this->bits_size > obj.bits_size) ? obj.array_size : this->array_size;

    for (unsigned long long iter(0); iter < array_size; ++iter)
    {
        this->bits[iter] &= obj.bits[iter];
    }

    this->bits[array_size - 1] &= this->mask[array_size - 1];

    return *this;
}

void my_bitset::operator++()
{
    for (uint64_t iter(0); iter < array_size; ++iter)
    {
        if (bits[iter] == mask[iter])
        {
            bits[iter] = 0;
        }
        else
        {
            bits[iter] += 1;
            break;
        }
    }
}

bool my_bitset::operator<(const my_bitset &obj)
{
    return static_cast<const my_bitset&>(*this) < obj;
}

bool my_bitset::operator<(const my_bitset &obj) const
{
    unsigned long long last_idx = (this->bits_size > obj.bits_size) ? obj.array_size : this->array_size;
    unsigned long long iter = 0;

    if (this->array_size > obj.array_size)
    {
        for (iter = this->array_size - 1; iter >= last_idx; --iter)
        {
            if (this->bits[iter] != bits_partition_t(0))
            {
                return false;
            }
        }
    }
    else if (this->array_size < obj.array_size)
    {
        for (iter = this->array_size - 1; iter >= last_idx; --iter)
        {
            if (obj.bits[iter] != bits_partition_t(0))
            {
                return false;
            }
        }
    }

    for (iter = last_idx - 1; iter > 0; --iter)
    {
        if (this->bits[iter] != obj.bits[iter])
        {
            break;
        }
    }

    if (this->bits[iter] < obj.bits[iter])
    {
        return true;
    }

    return false;
}