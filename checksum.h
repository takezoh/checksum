#ifndef takezoh_checksum_h
#define takezoh_checksum_h

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t checksum(const uint8_t* data, size_t size);
uint16_t checksum_with_prefix(uint16_t prefix, const uint8_t *data, size_t size);
uint16_t ones_complement_sum(uint16_t prefix, const uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif
