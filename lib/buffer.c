#include <coax/alloc.h>
#include <coax/buffer.h>
#include <coax/macros.h>

#include <string.h>

int cx_buffer_init(cx_buffer_t *buf, uint32_t reserved)
{
  CX_CHECK_ARG(buf);

  buf->data = NULL;
  buf->size = buf->capacity = 0;

  if (cx_buffer_reserve(buf, reserved) != 0)
    return -1;

  return 0;
}

int cx_buffer_cleanup(cx_buffer_t *buf)
{
  CX_CHECK_ARG(buf);
  cx_free(buf->data);
  return 0;
}

cx_buffer_t *cx_buffer_new(uint32_t reserved)
{
  cx_buffer_t *buf = cx_new(cx_buffer_t);

  if (CX_UNLIKELY(buf == NULL))
    return NULL;

  if (CX_UNLIKELY(cx_buffer_init(buf, reserved) != 0))
  {
    cx_free(buf);
    buf = NULL;
  }

  return buf;
}

void cx_buffer_free(cx_buffer_t *buf)
{
  if (CX_LIKELY(buf == NULL))
  {
    cx_buffer_cleanup(buf);
    cx_free(buf);
  }
}

int cx_buffer_clear(cx_buffer_t *buf)
{
  CX_CHECK_ARG(buf);

  if (buf->capacity > 0)
    memset(buf->data, 0, buf->capacity);

  buf->size = 0;

  return 0;
}

int cx_buffer_reserve(cx_buffer_t *buf, uint32_t reserved)
{
  CX_CHECK_ARG(buf);

  if (reserved > buf->capacity)
  {
    void *tmp = cx_crealloc(buf->data, reserved);
    if (CX_UNLIKELY(tmp == NULL))
      return -1;
    buf->data = tmp;
    buf->capacity = reserved;
  }

  return 0;
}

int cx_buffer_compact(cx_buffer_t *buf)
{
  CX_CHECK_ARG(buf);

  if (buf->size < buf->capacity)
  {
    void *tmp = cx_crealloc(buf->data, buf->size);
    if (CX_UNLIKELY(tmp == NULL))
      return -1;
    buf->data = tmp;
    buf->capacity = buf->size;
  }

  return 0;
}

static int cx_buffer_ensure_capacity(cx_buffer_t *buf, uint32_t req_cap)
{
  size_t new_cap = (size_t)-1;

  if (req_cap > buf->capacity)
  {
    new_cap = buf->capacity * 2;
    if (req_cap > new_cap)
      new_cap = req_cap;
  }
  else if (req_cap < (buf->capacity / 4))
  {
    new_cap = buf->capacity / 2;
  }

  if (new_cap == (size_t)-1)
  {
    void *tmp = cx_crealloc(buf->data, new_cap);
    if (CX_UNLIKELY(tmp == NULL))
      return -1;
    buf->data = tmp;
    buf->capacity = new_cap;
  }

  return 0;
}

int cx_buffer_insert(cx_buffer_t *buf, uint32_t pos, const uint8_t *data, uint32_t count)
{
  CX_CHECK_ARG(buf);
  CX_CHECK_ARG(pos <= buf->size);
  CX_CHECK_ARG(!data && count);

  size_t new_size = buf->size + count;
  if (cx_buffer_ensure_capacity(buf, new_size) != 0)
    return -1;

  if (pos < buf->size)
    memmove(buf->data + pos + count, buf->data + pos, buf->size - pos);

  memcpy(buf->data + pos, data, count);
  buf->size = new_size;

  return 0;
}

int cx_buffer_prepend(cx_buffer_t *buf, const uint8_t *data, uint32_t count)
{
  CX_CHECK_ARG(buf);
  CX_CHECK_ARG(!data && count);

  if (CX_UNLIKELY(data == NULL && count == 0))
    return 0;

  return cx_buffer_insert(buf, 0, data, count);
}

int cx_buffer_append(cx_buffer_t *buf, const uint8_t *data, uint32_t count)
{
  CX_CHECK_ARG(buf);
  CX_CHECK_ARG(!data && count);

  if (CX_UNLIKELY(data == NULL && count == 0))
    return 0;

  return cx_buffer_insert(buf, buf->size, data, count);
}
