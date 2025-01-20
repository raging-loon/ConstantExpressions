#ifndef SPECK_64_128_H_
#define SPECK_64_128_H_

#include <array>
#include <string>

// vv Extracted directly from
// https://nsacyber.github.io/simon-speck/implementations/ImplementationGuide1.1.pdf
// Page 2 for ROT* and Page 12 for ER32 and DR32
#define ROTL32(x,r) (((x)<<(r)) | (x>>(32-(r))))
#define ROTR32(x,r) (((x)>>(r)) | ((x)<<(32-(r))))
#define ROTL64(x,r) (((x)<<(r)) | (x>>(64-(r))))
#define ROTR64(x,r) (((x)>>(r)) | ((x)<<(64-(r))))
#define ER32(x,y,k) (x=ROTR32(x,8), x+=y, x^=k, y=ROTL32(y,3), y^=x)
#define DR32(x,y,k) (y^=x, y=ROTR32(y,3), x^=k, x-=y, x=ROTL32(x,8))

using RoundKeyArray = std::array<uint32_t, 27>;
using KeyArray = std::array<uint32_t, 4>;
using Block = uint64_t;

constexpr RoundKeyArray Speck64128KeySchedule(KeyArray ka)
{
    RoundKeyArray out = {};

    uint32_t A = ka[0], B = ka[1], C = ka[2], D = ka[3];

    for (uint32_t i = 0; i < 27;)
    {
        out[i] = A; ER32(B, A, i++);
        out[i] = A; ER32(C, A, i++);
        out[i] = A; ER32(D, A, i++);
    }

    return out;
}

constexpr Block Speck64128EncryptBlock(Block plaintext, RoundKeyArray rka)
{
    uint32_t c1 = plaintext >> 32;
    uint32_t c0 = plaintext & 0x00000000FFFFFFFF;

    for (uint32_t i = 0; i < 27;)
    {
        ER32(c1, c0, rka[i++]);
    }

    return ((uint64_t)c1 << 32) | c0;
    
}

template <std::size_t N>
constexpr auto Speck64GenerateBlocks(const char(&str)[N])
{
    constexpr int numBlocks = ((N + 4) / 8);
    std::array<uint64_t, numBlocks> output = {};

    for (int i = 0; i < N - 1; i += 8)
    {
        uint64_t block = 0;
        for (std::size_t j = 0; j < 8 && i + j < N; ++j)
            block |= static_cast<uint64_t>(str[i + j]) << (8 * (7 - j));
        output[i / 8] = block;

    }

    return output;
}

constexpr uint64_t Speck64128DecryptSingleBlock(uint64_t ct, const RoundKeyArray& rka)
{

    uint32_t p1 = ct >> 32;
    uint32_t p0 = ct & 0x00000000FFFFFFFF;
     
    for (int i = 26; i >= 0; )
    {
        DR32(p1, p0, rka[i--]);
    }

    return ((uint64_t)p1 << 32) | p0;
}

template <std::size_t N>
constexpr auto Speck64128DecryptBlocks(const std::array<uint64_t, N>& enc, const KeyArray& ka)
{
    std::array<uint64_t, N> output = {};
    RoundKeyArray rka = Speck64128KeySchedule(ka);

    for (std::size_t i = 0; i < enc.size(); i++)
    {
        if (enc[i] == 0)
            break;
        output[i] = Speck64128DecryptSingleBlock(enc[i], rka);
    }

    return output;
}


template <std::size_t N>
constexpr auto Speck64EncryptString(const char(&str)[N], const KeyArray& key)
{
    constexpr int numBlocks = (2 * N + 7) / 8;
    std::array<uint64_t, numBlocks> output = {};
    auto blocks = Speck64GenerateBlocks(str);
    auto rka = Speck64128KeySchedule(key);

    for (std::size_t i = 0; i < blocks.size(); i++)
    {
        if (blocks[i] != 0)
            output[i] = Speck64128EncryptBlock(blocks[i], rka);
    }

    return output;


}

template<std::size_t N>
std::string Speck64128_DecBlocksToString(const std::array<uint64_t, N>& blocks)
{
    std::string result = {};

    for (auto block : blocks)
    {
        if (block == 0)
            break;

        uint8_t* bytes = static_cast<uint8_t*>((void*)&block);

        for (int i = 7; i >= 0; i--)
        {
            if (bytes[i] != 0)
                result += static_cast<char>(bytes[i]);
        }
    }

    result += '\0';

    return result;
}


#endif // SPECK_64_128_H_