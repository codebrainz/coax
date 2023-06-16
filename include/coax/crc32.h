#ifndef CX_CRC32_H
#define CX_CRC32_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <stdint.h>

uint32_t cx_crc32(uint32_t crc32c, const uint8_t *buffer, uint32_t length);

#endif // CX_CRC32_H
