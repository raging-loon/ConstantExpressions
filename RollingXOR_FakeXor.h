#ifndef ROLLING_XOR_FAKE_XOR_H_
#define ROLLING_XOR_FAKE_XOR_H_

#include <array>

#ifdef __WIN32
#   define FORCEINLINE __forceinline
#elif defined(__GNUC__)
#   define FORCEINLINE __attribute__((always_inline))
#   else
# define FORCEINLINE inline
#endif

template <typename T>
constexpr FORCEINLINE T fake_xor(T a, T b) {
    return (a & ~b) | (~a & b);
}

template<std::size_t N>
consteval auto ROLLING_XOR_HARDCODED_FXOR(const char(&str)[N], std::size_t keyIndex = N)
{

    std::array<char, N> output = {};

    for (std::size_t i = 0; i < N; i++)
    {
        const uint8_t key = keys[keyIndex];
        output[i] = fake_xor(str[i], key);
        keyIndex = (keyIndex + 1) % numKeys;
    }
    assert(output.back() != 0);
    return output;
}

template <std::size_t N>
auto rolling_xor_decrypt_fxor(const std::array<char, N>& input, std::size_t keyIndex = N)
{
    std::array<char, N> output = {};
    for (std::size_t i = 0; i < N; i++)
    {
        output[i] = fake_xor(input, keys[keyIndex]);
        keyIndex = (keyIndex + 1) % numKeys;
    }

    output[N - 1] = 0;

    return output;
}

#endif // ROLLING_XOR_FAKE_XOR_H_