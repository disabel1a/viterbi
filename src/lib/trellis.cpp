#include "../include/trellis.h"

#include <iostream>
#include <queue>

trellis::trellis(uint64_t reg_size, std::vector<std::vector<my_bitset>> &generate_matrix, uint64_t height, uint64_t width) : 
    reg_size(reg_size),
    states_num(1 << (reg_size * height)),
    state_size(reg_size * height),
    code_rate(width / height),
    height(height),
    width(width)
{
    states = new state[states_num];

    my_bitset input_bits(height, 0);
    my_bitset output_bits(width, 0);
    my_bitset next_state(state_size, 0);
    my_bitset out_function(reg_size + 1, 0);

    my_bitset next_reg(reg_size, 0);

    //////////////////

    std::cout << "input_bits: " << input_bits.to_string()<< std::endl;
    std::cout << "output_bits: " << output_bits.to_string() << std::endl;
    std::cout << "next_state: " << next_state.to_string() << std::endl;
    std::cout << "out_function " << out_function.to_string() << std::endl;
    std::cout << "next_reg: " << next_reg.to_string() << std::endl;

    //////////////////

    uint64_t counter(0);

    for (uint64_t iter(0); iter < states_num; ++iter)
    {
        states[iter].set_index(iter);
        std::cout << "CURRENT STATE: " << iter << std::endl;

        for (uint64_t input_value(0); input_value < (uint64_t(1) << height); ++input_value)
        {
            std::cout << "NEW INPUT:" << std::endl;
            input_bits = input_value;
            output_bits.reset();
            next_state.reset();
            out_function.reset();

            std::cout << "input_bits: " << input_bits.to_string()<< std::endl;
            std::cout << "output_bits: " << output_bits.to_string() << std::endl;
            std::cout << "next_state: " << next_state.to_string() << std::endl;
            std::cout << "out_function " << out_function.to_string() << std::endl;

            std::cout << std::endl;

            for (uint64_t out_bit(0); out_bit < width; ++out_bit)
            {
                counter = 0;
                for (uint64_t in_bit(0); in_bit < height; ++in_bit)
                {
                    out_function = (iter >> (in_bit * reg_size)) << 1;
                    if (input_bits.at(in_bit))
                    {
                        out_function.set(0);
                    }

                    out_function &= generate_matrix[in_bit][out_bit];
                    std::cout << "out_function " << out_function.to_string() << std::endl;

                    counter += out_function.count();
                }

                if (counter & 1)
                {
                    output_bits.set(out_bit);
                }

                std::cout << "output_bits: " << output_bits.to_string() << std::endl << std::endl;
            }

            for (uint64_t reg_idx(height - 1); reg_idx >= 0; --reg_idx)
            {
                next_reg = (iter >> (reg_idx * reg_size));
                next_reg <<= 1;
                if (input_bits.at(reg_idx))
                {
                    next_reg.set(0);
                }

                std::cout << "next_reg: " << next_reg.to_string() << std::endl;

                next_state ^= next_reg;
                next_state <<= reg_idx * reg_size;

                std::cout << "next_state: " << next_state.to_string() << std::endl << std::endl;

                if (reg_idx == 0)
                {
                    break;
                }
            }

            std::pair<state *, my_bitset> p{&states[next_state.to_ull()], output_bits};
            
            states[iter].get_next_states().insert({input_bits, p});
        }

        std::cout << "END STATE" << std::endl << std::endl;
    }
}

trellis::~trellis()
{
    delete [] states;
}

my_bitset trellis::code(my_bitset msg)
{
    my_bitset inputs(height, 0);
    my_bitset outputs(width, 0);
    my_bitset codeword(((msg.get_size() / height) * width), 0);

    state *current_state = &states[0];

    for (uint64_t iter(0); iter < (msg.get_size() / height); ++iter)
    {
        inputs = msg;
        msg >>= height;

        outputs = current_state->get_next_states().at(inputs).second;
        current_state = current_state->get_next_states().at(inputs).first;

        codeword <<= width;
        codeword ^= outputs;
    }
    //std::cout << "CODE_REV: " << codeword.to_string() << std::endl;
    codeword.reverse_bits(width);
    return codeword;
}

my_bitset trellis::decode(my_bitset msg)
{
    std::vector<uint64_t> metrics(states_num, {0});
    std::vector<uint64_t> next_metrics(states_num, {0});

    uint64_t decoded_msg_size = (msg.get_size() / width) * height;

    std::vector<my_bitset> paths(states_num, {decoded_msg_size, 0});
    std::vector<my_bitset> next_paths(states_num, {decoded_msg_size, 0});

    std::map<state *, bool> avaliable_states;
    avaliable_states.insert({states, false});

    std::map<state *, bool> next_states;

    my_bitset input_dec(width, 0);

    my_bitset input_bits(height, 0);
    my_bitset output_bits(width, 0);

    state *current_state = nullptr;

    my_bitset local_path(decoded_msg_size, 0);

    for (uint64_t iter(0); iter < (msg.get_size() / width); ++iter)
    {
        input_dec = msg;
        while (!avaliable_states.empty())
        {
            current_state = avaliable_states.begin()->first;
            avaliable_states.erase(current_state);

            //uint64_t state_idx = states - current_state;

            for (uint64_t input_value(0); input_value < (uint64_t(1) << height); ++input_value)
            {
                input_bits = input_value;

                output_bits = current_state->get_next_states().at(input_bits).second;
                state *next_state = current_state->get_next_states().at(input_bits).first;

                //uint64_t next_state_idx = states - next_state;

                output_bits ^= input_dec;

                uint64_t local_metric = output_bits.count() + metrics[current_state->get_index()];

                if ((next_states.find(next_state) == next_states.end()) || local_metric < next_metrics[next_state->get_index()])
                {
                    next_metrics[next_state->get_index()] = local_metric;

                    local_path = paths[current_state->get_index()];
                    local_path <<= height;
                    local_path ^= input_bits;

                    next_paths[next_state->get_index()] = local_path;

                    next_states.insert({next_state, false});
                }
            }
        }
        
        while (!next_states.empty())
        {
            avaliable_states.insert({next_states.begin()->first, false});
            next_states.erase(next_states.begin()->first);
        }

        metrics = next_metrics;
        paths = next_paths;

        msg >>= width;
    }

    uint64_t min_metric = UINT64_MAX;
    uint64_t min_metric_state_idx = 0;

    for (uint64_t iter(0); iter < states_num; ++iter)
    {
        //std::cout << "p" << iter << ": " << paths[iter].to_string() << std::endl;
        if (metrics[iter] < min_metric)
        {
            min_metric = metrics[iter];
            min_metric_state_idx = iter;
        }
    }

    paths[min_metric_state_idx].reverse_bits(height);

    return paths[min_metric_state_idx];
}