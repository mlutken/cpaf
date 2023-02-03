#include "md5sum.h"

#include <cstdint>
#include <cstring>
#include <iomanip>
#include <sstream>

namespace cpaf {
namespace crypto {

void md5sum::state::update(const char* chunk) {
    std::array<uint32_t, 16> m;
    for(auto i = 0u; i < 16; ++i) {
        m[i] =
                ((uint32_t)(uint8_t)chunk[i * 4]) |
                ((uint32_t)(uint8_t)chunk[i * 4 + 1] << 8) |
                ((uint32_t)(uint8_t)chunk[i * 4 + 2] << 16) |
                ((uint32_t)(uint8_t)chunk[i * 4 + 3] << 24);
    }
    uint32_t a2 = hash_[0], b2 = hash_[1], c2 = hash_[2], d2 = hash_[3];
    for(uint32_t i = 0; i < 64; ++i) {
        uint32_t f;
        uint32_t g;
        if(i < 16) {
            f = (b2 & c2) | (~b2 & d2);
            g = i;
        } else if(i < 32) {
            f = (d2 & b2) | (~d2 & c2);
            g = (5 * i + 1) % 16;
        } else if(i < 48) {
            f = b2 ^ c2 ^ d2;
            g = (3 * i + 5) % 16;
        } else {
            f = c2 ^ (b2 | ~d2);
            g = 7 * i % 16;
        }
        uint32_t d3 = d2;
        d2 = c2;
        c2 = b2;
        b2 += rotl(a2 + f + K[i] + m[g], S[i]);
        a2 = d3;
    }
    hash_[0] += a2;
    hash_[1] += b2;
    hash_[2] += c2;
    hash_[3] += d2;
}

void md5sum::write(size_t datasize, const char* data) {
    while(datasize > 0) {
        auto wrsize = std::min<size_t>(64 - position_ % 64, datasize);
        std::memcpy(&bufffer_[position_ % 64], data, wrsize);
        datasize -= wrsize;
        data += wrsize;
        position_ += wrsize;
        if(position_ % 64 == 0) {
            state_.update(&bufffer_[0]);
        }
    }
}

std::array<char, md5sum::result_size> md5sum::sum() const {
    auto state2 = state_;
    auto buf2 = bufffer_;
    size_t pos = position_ % 64u;
    buf2[pos] = static_cast<char>(0x80);
    if(pos <= 55) {
        std::fill(&buf2[pos + 1], &buf2[56], 0);
    } else {
        std::fill(&buf2[pos + 1], &buf2[64], 0);
        state2.update(&buf2[0]);
        std::fill(&buf2[0], &buf2[56], 0);
    }
    for(auto i = 0u; i < 8; ++i) {
        buf2[56u + i] = (position_ * 8) >> (8 * i);
    }
    state2.update(&buf2[0]);
    std::array<char, result_size> ret;
    for(auto i = 0u; i < 16u; ++i) {
        ret[i] = state2.hash_[i / 4] >> (8 * (i % 4));
    }
    return ret;
}

std::string md5sum::sum_hex() const
{
    std::stringstream ss;
    ss << std::setfill('0') << std::hex;
    const auto& res_sum = sum();
    for(std::size_t i= 0; i < md5sum::result_size; ++i) {
        ss << std::setw(sizeof(res_sum[0])*2) << res_sum[i];
    }
    return ss.str();

}


} // namespace crypto
} // namespace cpaf
