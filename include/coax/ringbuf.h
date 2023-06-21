#ifndef CX_RINGBUF_H
#define CX_RINGBUF_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <coax/buffer.h>

typedef struct cx_ringbuf cx_ringbuf_t;

struct cx_ringbuf
{
  cx_buffer_t buf;
  uint32_t roff;
  uint32_t woff;
  uint32_t len;
};

int cx_ringbuf_init(cx_ringbuf_t *rbuf, uint32_t size);
int cx_ringbuf_cleanup(cx_ringbuf_t *rbuf);

cx_ringbuf_t *cx_ringbuf_new(uint32_t size);
void cx_ringbuf_free(cx_ringbuf_t *rbuf);

int cx_ringbuf_clear(cx_ringbuf_t *rbuf);

int cx_ringbuf_push(cx_ringbuf_t *rbuf, const uint8_t *data, uint32_t count);
int cx_ringbuf_pop(cx_ringbuf_t *rbuf, uint8_t *data, uint32_t *count);

#endif // CX_RINGBUF_H
