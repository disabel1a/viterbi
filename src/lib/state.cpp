#include "../include/state.h"

state::state() : index(0), next_states()
{
}

state::~state()
{

}

std::map<my_bitset, std::pair<state *, my_bitset>> &state::get_next_states()
{
    return next_states;
}

uint64_t state::get_index()
{
    return index;
}

void state::set_index(uint64_t index)
{
    this->index = index;
}