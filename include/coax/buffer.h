#ifndef CX_BUFFER_H
#define CX_BUFFER_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <stdint.h>

typedef struct cx_buffer cx_buffer_t;

struct cx_buffer
{
  uint8_t *data;
  uint32_t size;
  uint32_t capacity;
};

int cx_buffer_init(cx_buffer_t *buf, uint32_t reserved);
int cx_buffer_cleanup(cx_buffer_t *buf);
cx_buffer_t *cx_buffer_new(uint32_t reserved);
void cx_buffer_free(cx_buffer_t *buf);

int cx_buffer_clear(cx_buffer_t *buf);
int cx_buffer_reserve(cx_buffer_t *buf, uint32_t reserved);
int cx_buffer_compact(cx_buffer_t *buf);

int cx_buffer_insert(cx_buffer_t *buf, uint32_t pos, const uint8_t *data, uint32_t count);
int cx_buffer_prepend(cx_buffer_t *buf, const uint8_t *data, uint32_t count);
int cx_buffer_append(cx_buffer_t *buf, const uint8_t *data, uint32_t count);

#endif // CX_BUFFER_H
