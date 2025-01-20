#ifndef ROLLING_XOR_H_
#define ROLLING_XOR_H_

#include "XORBasic.h"
#include <cassert>

constexpr std::array<uint8_t, 64> keys = {
    156, 38, 250, 86, 196, 106, 137, 193, 4, 123, 79, 114, 240, 72, 115, 55,
    235, 40, 8, 222, 139, 69, 88, 107, 227, 234, 128, 210, 226, 142, 62, 251,
    26, 75, 116, 206, 135, 138, 33, 60, 229, 161, 236, 221, 198, 248, 155, 41,
    39, 30, 83, 177, 157, 145, 65, 134, 243, 162, 29, 67, 45, 205, 118, 15
};
constexpr std::size_t numKeys = keys.size();

template<std::size_t N>
consteval auto ROLLING_XOR_HARDCODED(const char(&str)[N], std::size_t keyIndex = N)
{

    std::array<char, N> output = {};

    for (std::size_t i = 0; i < N; i++)
    {
        const uint8_t key = keys[keyIndex];
        output[i] = str[i] ^ key;
        keyIndex = (keyIndex + 1) % numKeys;
    }
    assert(output.back() != 0);
    return output;
}

template <std::size_t N>
auto rolling_xor_decrypt(const std::array<char, N>& input, std::size_t keyIndex = N)
{
    std::array<char, N> output = {};
    for (std::size_t i = 0; i < N; i++)
    {
        output[i] = input[i] ^ keys[keyIndex];
        keyIndex = (keyIndex + 1) % numKeys;
    }

    output[N - 1] = 0;

    return output;
}

#endif // ROLLING_XOR_H_