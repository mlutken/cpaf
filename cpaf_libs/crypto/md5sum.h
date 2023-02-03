#pragma once

#include <string>
#include <array>

namespace cpaf {
namespace crypto {

class md5sum {
private:
    struct state {
        static constexpr std::array<int, 64> S = {
            7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
            5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
            4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
            6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
        };

        static constexpr std::array<uint32_t, 64> K = {
            0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE,
            0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
            0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE,
            0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
            0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA,
            0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
            0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED,
            0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
            0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C,
            0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
            0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05,
            0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
            0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039,
            0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
            0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1,
            0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391
        };

        static inline uint32_t rotl(uint32_t x, int sh) {
            return (x << sh) | (x >> (32-sh));
        }

        state() : hash_({0, 0, 0, 0}) {};

        state(std::array<uint32_t, 4> h) : hash_(h) {}

        void update(const char* chunk);

        std::array<uint32_t, 4> hash_;
    };


public:
    static constexpr uint8_t result_size = 16;
    using result_type = std::array<char, result_size>;

    md5sum() :
        state_({0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476}),
        position_(0), bufffer_() {}

    void write(std::string_view str) {
        write(str.size(), str.data());
    }

    void write(size_t datasize, const char* data);

    std::array<char, result_size> sum() const;

    std::string sum_hex() const;


private:

    state state_;
    uint64_t position_;
    std::array<char, 64> bufffer_;
};

} // namespace crypto
} // namespace cpaf

