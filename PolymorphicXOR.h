#ifndef POLYMORPHIC_ENCRYPTION_H_
#define POLYMORPHIC_ENCRYPTION_H_

#include "KeyGen.h"

template <std::size_t N>
consteval auto POLY_XOR_ENCRYPT(const char(&str)[N])
{
    std::array<char, N> output = {};
    uint8_t key;
    if constexpr(N >= 16)
        key = LCG(N);
    else
        key = MIDDLE_SQUARE(N);

    for (std::size_t i = 0; i < N; i++)
        output[i] = str[i] ^ key;

    return output;
}

#endif // POLYMORPHIC_ENCRYPTION_H_