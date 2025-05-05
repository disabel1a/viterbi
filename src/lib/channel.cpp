#include "../include/channel.h"

#include <random>

channel::channel(double prob) : prob(prob)
{

}

channel::~channel()
{

}

void channel::set_prob(double prob)
{
    this->prob = prob;
}

my_bitset channel::add_noise(my_bitset msg)
{
    my_bitset noise_mask(msg.get_size(), 0);

    double rand_prob = 0.0;

    for (uint64_t iter(0); iter < noise_mask.get_size(); ++iter)
    {
        noise_mask <<= 1;
        rand_prob = static_cast<double>(random()) / static_cast<double>(RAND_MAX);

        if (rand_prob <= prob)
        {
            ++noise_mask;
        }
    }

    msg ^= noise_mask;

    return msg;
}