#ifndef KEY_GEN_H_
#define KEY_GEN_H_

#include <array>

constexpr uint8_t LCG(uint8_t seed)
{
    constexpr uint8_t a = 75, c = 76, m = 255;
    
    return (a * seed + c) % m;
}

constexpr uint8_t MIDDLE_SQUARE(uint8_t seed)
{
    return ((seed * seed) >> 4) & 0xff;
}

template <std::size_t N>
consteval auto ROLLING_XOR_KEY_GEN(const char(&str)[N])
{
    std::array<char, N> output = {};

    const uint8_t key = LCG(N);

    for (std::size_t i = 0; i < N; i++)
        output[i] = str[i] ^ key;

    return output;
}

template <std::size_t N>
consteval auto ROLLING_XOR_KEY_GEN2(const char(&str)[N])
{
    std::array<char, N> output = {};

    const uint8_t key = MIDDLE_SQUARE(N);

    for (std::size_t i = 0; i < N; i++)
        output[i] = str[i] ^ key;

    return output;
}


#endif // KEY_GEN_H_