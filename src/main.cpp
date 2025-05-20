#include "include/my_bitset.h"
#include "include/trellis.h"
#include "include/channel.h"
#include "include/file_tools.h"

#include <iostream>
#include <random>

uint64_t reg_size = 3;

// In matlab 13 is 11; 11 is 13 (decimal)
// 13: 'b1101 -> 'b1011 = 11
// 11: 'b1011 -> 'b1101 = 13
// 15: 'b1111 -> 'b1111 = 15
my_bitset poly13(reg_size + 1, 13);
my_bitset poly11(reg_size + 1, 11);
my_bitset poly15(reg_size + 1, 15);

std::vector<std::vector<my_bitset>> matrix_1_by_2 = {
                                                        {poly11, poly15}
                                                    };

std::vector<std::vector<my_bitset>> matrix_1_by_3 = {
                                                        {poly13, poly11, poly15}
                                                    };

std::vector<std::vector<my_bitset>> matrix_2_by_3 = {
                                                        {poly13, poly11, poly15},
                                                        {poly11, poly13, poly15}
                                                    };

std::vector<std::vector<my_bitset>> matrix_3_by_5 = {
                                                        {poly15, poly13, poly13, poly11, poly15},
                                                        {poly13, poly11, poly15, poly13, poly11},
                                                        {poly11, poly13, poly11, poly15, poly13}
                                                    };

std::vector<std::vector<my_bitset>> matrix_1_by_5 = {
                                                        {poly11, poly15, poly13, poly15, poly11}
                                                    };

enum class matrix_size {ONE_BY_TWO, ONE_BY_THREE, TWO_BY_THREE, THREE_BY_FIVE, ONE_BY_FIVE};

void trellis_test()
{
    trellis t1(reg_size, matrix_1_by_2, matrix_1_by_2.size(), matrix_1_by_2[0].size());
    trellis t2(reg_size, matrix_1_by_3, matrix_1_by_3.size(), matrix_1_by_3[0].size());
    trellis t3(reg_size, matrix_2_by_3, matrix_2_by_3.size(), matrix_2_by_3[0].size());

    // my_bitset msg("0101000100", base::BIN);
    my_bitset msg("01010001001110110110", base::BIN);
    std::cout << "MSG: " << msg.to_string() << std::endl;

    /////////////////////////////////////////////////////////////

    my_bitset code1 = t1.code(msg);
    std::cout << "CODE_RES: " << code1.to_string() << std::endl;

    my_bitset decoded_msg_1 = t1.decode(code1);
    std::cout << "DECODED: " << decoded_msg_1.to_string() << std::endl;
    std::cout << "MSG_RES: " << msg.to_string() << std::endl << std::endl;

    /////////////////////////////////////////////////////////////

    std::cout << "MSG: " << msg.to_string() << std::endl;

    my_bitset code2 = t2.code(msg);
    std::cout << "CODE_RES: " << code2.to_string() << std::endl;

    my_bitset decoded_msg_2 = t2.decode(code2);
    std::cout << "DECODED: " << decoded_msg_2.to_string() << std::endl;
    std::cout << "MSG_RES: " << msg.to_string() << std::endl << std::endl;

    /////////////////////////////////////////////////////////////

    std::cout << "MSG: " << msg.to_string() << std::endl;

    my_bitset code3 = t3.code(msg);
    std::cout << "CODE_RES: " << code3.to_string() << std::endl;

    my_bitset decoded_msg_3 = t3.decode(code3);
    std::cout << "DECODED: " << decoded_msg_3.to_string() << std::endl;
    std::cout << "MSG_RES: " << msg.to_string() << std::endl << std::endl;
}

void heavy_matrix_test()
{
    trellis t(reg_size, matrix_3_by_5, matrix_3_by_5.size(), matrix_3_by_5[0].size());

    my_bitset msg("010100010011101101101", base::BIN);
    std::cout << "MSG: " << msg.to_string() << std::endl;

    my_bitset code = t.code(msg);
    std::cout << "CODE_RES: " << code.to_string() << std::endl;

    my_bitset decoded_msg = t.decode(code);
    std::cout << "DECODED: " << decoded_msg.to_string() << std::endl;
    std::cout << "MSG_RES: " << msg.to_string() << std::endl << std::endl;
}

void channel_test()
{
    double probability = 0.2;

    channel ch(probability);
    trellis t(reg_size, matrix_1_by_5, matrix_1_by_5.size(), matrix_1_by_5[0].size());

    my_bitset msg("0101000100", base::BIN);
    std::cout << "MSG: " << msg.to_string() << std::endl;

    my_bitset code = t.code(msg);
    std::cout << "CODE_RES: " << code.to_string() << std::endl;

    my_bitset channel_code = ch.add_noise(code);
    std::cout << "CHL_CODE: " << channel_code.to_string() << std::endl;

    my_bitset decoded_msg = t.decode(channel_code);
    std::cout << "DECODED: " << decoded_msg.to_string() << std::endl;
    std::cout << "MSG_RES: " << msg.to_string() << std::endl << std::endl;
}

std::pair<std::vector<double>, std::vector<double>> berr_check_model(uint64_t interations, double thr, double duration, matrix_size m_size, uint64_t msg_size, bool guard = false)
{
    std::vector<double> berr_vec;
    std::vector<double> dur_points;

    double berr = 0.0;

    std::string msg;
    msg.reserve(msg_size);

    std::vector<std::vector<my_bitset>> *matrix;
    switch (m_size)
    {
    case matrix_size::ONE_BY_TWO:
        matrix = &matrix_1_by_2;
        break;

    case matrix_size::ONE_BY_THREE:
        matrix = &matrix_1_by_3;
        break;

    case matrix_size::ONE_BY_FIVE:
        matrix = &matrix_1_by_5;
        break;

    case matrix_size::TWO_BY_THREE: 
        matrix = &matrix_2_by_3;
        break;

    case matrix_size::THREE_BY_FIVE:
        matrix = &matrix_3_by_5;
        break;

    default:
        matrix = &matrix_1_by_2;
        break;
    }

    trellis t(reg_size, (*matrix), (*matrix).size(), (*matrix)[0].size());
    channel ch(0.0);

    // Guard iterval for convolution
    uint64_t height = (*matrix).size();
    uint64_t guard_len = (guard) ? (height * reg_size) + ((msg_size + reg_size) % height) : 0;

    for (double prob(0.0); prob <= thr; prob += duration)
    {
        berr = 0.0;
        ch.set_prob(prob);

        for (uint64_t iter(0); iter < interations; ++iter)
        {
            for (uint64_t symbol_num(0); symbol_num < msg_size; ++symbol_num)
            {
                char symbol = (static_cast<double>(random()) / static_cast<double>(RAND_MAX)) >= 0.5 ? '1' : '0';
                msg += symbol;
            }

            for (uint64_t symbol_num(0); symbol_num < guard_len; ++symbol_num)
            {
                msg += '0';
            }

            my_bitset msg_bits(msg, base::BIN);

            my_bitset codeword = t.code(msg_bits);

            my_bitset noised_codeword = ch.add_noise(codeword);

            my_bitset decoded_msg = t.decode(noised_codeword);

            berr += static_cast<double>((decoded_msg ^ msg_bits).count()) / static_cast<double>(msg_size);
            msg.clear();
        }

        berr /= static_cast<double>(interations);

        berr_vec.push_back(berr);
        dur_points.push_back(prob);
    }

    return std::make_pair(berr_vec, dur_points);
}

int main(/*int argc, char const *argv[]*/)
{
    srand(10);
    //trellis_test();
    //heavy_matrix_test();
    //channel_test();

    auto res = berr_check_model(1000, 1.01, 0.01, matrix_size::ONE_BY_FIVE, 20, true);

    std::string data_path = "data";
    std::string folder_name = "/one_by_five";

    std::string berr_file_path = data_path + folder_name + "/berr.txt";
    std::string dur_file_path = data_path + folder_name + "/duration.txt";

    check_for_file(berr_file_path.c_str());
    check_for_file(dur_file_path.c_str());

    write_vector_file(berr_file_path, res.first);
    write_vector_file(dur_file_path, res.second);

    return 0;
}
