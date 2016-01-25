#include "checksum.h"
#include <string.h>

namespace {
    inline uint64_t SUM(const uint64_t &chunk) {
        uint64_t tmp = (chunk >> 16 & 0x0000ffff0000ffff) + (chunk & 0x0000ffff0000ffff);
        return (tmp >> 32) + (tmp & 0x00000000ffffffff);
    }

    inline uint32_t SUM(const uint32_t &chunk) {
        return (chunk >> 16) + (chunk & 0x0000ffff);
    }

    template <typename T>
    uint16_t ones_complement_sum(uint16_t prefix, const uint8_t* data, size_t size) {
        T sum = prefix;
        const T *p = reinterpret_cast<const T*>(data);
        for (; size >= sizeof(T); ++p, size -= sizeof(T)) {
            sum += SUM(*p);
            if (sum & (1UL << (sizeof(T) * 8 - 1))) {
                sum = SUM(sum);
            }
        }
        if (size > 0) {
            T chunk = 0;
            memcpy(&chunk, p, size);
            sum += SUM(chunk);
        }
        while (sum >> 16) {
            sum = SUM(sum);
        }
        return static_cast<uint16_t>(sum);
    }

    typedef uint16_t (*F)(uint16_t, const uint8_t*, size_t);
    template <int CPU_REGISTER_SIZE> constexpr F ones_complement_sum();
    template <> constexpr F ones_complement_sum<8>() { return ones_complement_sum<uint64_t>; }
    template <> constexpr F ones_complement_sum<4>() { return ones_complement_sum<uint32_t>; }
    constexpr F ONES_COMPLEMENT_SUM = ones_complement_sum<sizeof(void*)>();
}

extern "C" {
    uint16_t ones_complement_sum(uint16_t prefix, const uint8_t *data, size_t size) {
        return ONES_COMPLEMENT_SUM(prefix, data, size);
    }

    uint16_t checksum(const uint8_t* data, size_t size) {
        return ~ONES_COMPLEMENT_SUM(0, data, size);
    }

    uint16_t checksum_with_prefix(uint16_t prefix, const uint8_t *data, size_t size) {
        return ~ONES_COMPLEMENT_SUM(prefix, data, size);
    }
}
