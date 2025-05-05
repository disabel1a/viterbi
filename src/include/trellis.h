#ifndef TRELLIS
#define TRELLIS

#include <map>
#include <vector>

#include "my_bitset.h"
#include "state.h"

class trellis
{
private:
    state *states;
    uint64_t reg_size;
    uint64_t states_num;
    uint64_t state_size;
    uint64_t code_rate;

    uint64_t height;
    uint64_t width;

    uint64_t count_pow(uint64_t &val);

public:
    trellis(uint64_t reg_size, std::vector<std::vector<my_bitset>> &generate_matrix, uint64_t height, uint64_t width);
    ~trellis();

    my_bitset code(my_bitset msg);

    my_bitset decode(my_bitset msg);
};

#endif // !TRELLIS
