#ifndef CX_MD5_H
#define CX_MD5_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <stdint.h>
#include <stdio.h>

#define CX_MD5_DIGEST_LEN 16

typedef struct cx_md5 cx_md5_t;

struct cx_md5
{
  uint64_t size;                     // Size of input in bytes
  uint32_t buffer[4];                // Current accumulation of hash
  uint8_t input[64];                 // Input to be used in the next step
  uint8_t digest[CX_MD5_DIGEST_LEN]; // Result of algorithm
};

void cx_md5_init(cx_md5_t *ctx);
void cx_md5_update(cx_md5_t *ctx, const uint8_t *input, size_t input_len);
void cx_md5_finalize(cx_md5_t *ctx);
void cx_md5_step(uint32_t *buffer, const uint32_t *input);

void cx_md5_string(const char *input, uint8_t result[CX_MD5_DIGEST_LEN]);
void cx_md5_file(FILE *file, uint8_t result[CX_MD5_DIGEST_LEN]);

#endif // CX_MD5_H
