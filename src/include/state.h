#ifndef STATE
#define STATE

#include "my_bitset.h"

#include <map>

class state
{
private:
    uint64_t index;
    // <input : pair(state *, output)>
    std::map<my_bitset, std::pair<state *, my_bitset>> next_states;

public:
    state();
    ~state();

    std::map<my_bitset, std::pair<state *, my_bitset>> &get_next_states();
    uint64_t get_index();
    void set_index(uint64_t index);
};

#endif // !STATE