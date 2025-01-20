#ifndef XOR_BASIC_H_
#define XOR_BASIC_H_

#include <array>

constexpr unsigned int XOR_CONSTANT = 131;

///
/// PURPOSE
///     XOR Encrypt a string
/// 
/// PARAMS
///     str - the string
///     key - the key - default is XOR_CONSTANT
///
/// TYPE PARAMS
///     N - size of the string
///
/// RETURNS
///     std::array of N encrypted characters 
///
template<std::size_t N>
constexpr auto XOR_ENC_DEC_STR(const char (&str)[N], std::size_t key = XOR_CONSTANT)
{
    std::array<char, N> enc = {};
    for (std::size_t i = 0; i < N - 1; ++i)
        enc[i] = str[i] ^ key;
    enc[N - 1] = 0;
    return enc;
}

#endif // XOR_BASIC_H_