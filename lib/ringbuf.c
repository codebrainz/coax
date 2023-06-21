#include <coax/alloc.h>
#include <coax/macros.h>
#include <coax/ringbuf.h>

int cx_ringbuf_init(cx_ringbuf_t *rbuf, uint32_t size)
{
  CX_CHECK_ARG(rbuf);
  CX_CHECK_ARG(size > 1);

  if (cx_buffer_init((cx_buffer_t *)rbuf, size) != 0)
    return -1;

  rbuf->buf.size = size;
  rbuf->roff = rbuf->woff = rbuf->len = 0;

  return 0;
}

int cx_ringbuf_cleanup(cx_ringbuf_t *rbuf)
{
  CX_CHECK_ARG(rbuf);
  cx_buffer_cleanup((cx_buffer_t *)rbuf);
  return 0;
}

cx_ringbuf_t *cx_ringbuf_new(uint32_t size)
{
  CX_CHECK_ARG_RET_VAL(size > 1, NULL);

  cx_ringbuf_t *rbuf = cx_new(cx_ringbuf_t);

  if (CX_UNLIKELY(rbuf == NULL))
    return NULL;

  if (CX_UNLIKELY(cx_ringbuf_init(rbuf, size) != 0))
  {
    cx_free(rbuf);
    rbuf = NULL;
  }

  return rbuf;
}

void cx_ringbuf_free(cx_ringbuf_t *rbuf)
{
  CX_CHECK_ARG_NO_RETVAL(rbuf);
  cx_ringbuf_cleanup(rbuf);
  cx_free(rbuf);
}

int cx_ringbuf_clear(cx_ringbuf_t *rbuf)
{
  CX_CHECK_ARG(rbuf);
  rbuf->roff = rbuf->woff = rbuf->len = 0;
  return 0;
}

static void cx_ringbuf_push_b(cx_ringbuf_t *rbuf, uint8_t b)
{
  uint32_t off = rbuf->woff + 1;
  off = off % rbuf->buf.size;
  rbuf->buf.data[off] = b;
  rbuf->woff = off;
  rbuf->len++;
}

int cx_ringbuf_push(cx_ringbuf_t *rbuf, const uint8_t *data, uint32_t count)
{
  CX_CHECK_ARG(rbuf);
  CX_CHECK_ARG(data);
  CX_CHECK_ARG(count);

  if (rbuf->len + count > rbuf->buf.size)
  {
    errno = E2BIG;
    return -1;
  }

  for (uint32_t i = 0; i < count; i++)
    cx_ringbuf_push_b(rbuf, data[i]);

  return 0;
}

static void cx_ringbuf_pop_b(cx_ringbuf_t *rbuf, uint8_t *b)
{
  *b = rbuf->buf.data[rbuf->roff];
  rbuf->roff = (rbuf->roff + 1) % rbuf->buf.size;
  rbuf->len--;
}

int cx_ringbuf_pop(cx_ringbuf_t *rbuf, uint8_t *data, uint32_t *count)
{
  CX_CHECK_ARG(rbuf);
  CX_CHECK_ARG(data);
  CX_CHECK_ARG(count);

  if (CX_UNLIKELY(*count == 0))
    return 0;

  if (CX_UNLIKELY(rbuf->len == 0))
    return 0;

  uint32_t i = 0, cnt = *count;
  for (; i < cnt && rbuf->len > 0; i++)
    cx_ringbuf_pop_b(rbuf, &data[i]);
  *count = i;

  return 0;
}
