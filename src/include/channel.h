#ifndef CHANNEL
#define CHANNEL

#include "my_bitset.h"

class channel
{
private:
    double prob;

public:
    channel(double prob);
    ~channel();

    void set_prob(double prob);

    my_bitset add_noise(my_bitset msg);
};

#endif // CHANNEL