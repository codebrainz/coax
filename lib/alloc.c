#include <coax/alloc.h>
#include <coax/log.h>
#include <coax/macros.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cx_memhdr
{
  size_t sz;
};

#define CX_HDR(x) ((struct cx_memhdr *)(x))
#define CX_PTR2HDR(p) (CX_HDR(p) - 1)
#define CX_HDR2PTR(h) (CX_HDR(p) + 1)

#if CX_FATAL_OOM
#define CX_CHECK_OOM(p, func)                         \
  do                                                  \
  {                                                   \
    if (CX_UNLIKELY(!(p)))                            \
      cx_fatal("out of memory calling '" #func "'!"); \
  } while (0)
#else
#define CX_CHECK_OOM(p, func) \
  do                          \
  {                           \
  } while (0)
#endif

void *cx_malloc(size_t size)
{
  void *p = CX_MALLOC(size + sizeof(struct cx_memhdr));
  CX_CHECK_OOM(p, cx_malloc);
  if (CX_LIKELY(p))
  {
    CX_HDR(p)->sz = size;
    p = CX_HDR2PTR(p);
  }
  return p;
}

void *cx_calloc(size_t n, size_t size)
{
  const size_t total = n * size;
  void *p = cx_malloc(total);
  cx_memclear(p);
  return p;
}

void *cx_realloc(void *oldp, size_t size)
{
  if (CX_UNLIKELY(oldp == NULL))
    return NULL;
  void *p = CX_REALLOC(CX_PTR2HDR(oldp), size);
  CX_CHECK_OOM(p, cx_realloc);
  if (CX_LIKELY(p))
  {
    CX_HDR(p)->sz = size;
    p = CX_HDR2PTR(p);
  }
  return p;
}

void *cx_crealloc(void *oldp, size_t size)
{
  if (CX_UNLIKELY(oldp == NULL))
    return NULL;
  size_t old_size = CX_PTR2HDR(oldp)->sz;
  void *p = CX_REALLOC(CX_PTR2HDR(oldp), size);
  CX_CHECK_OOM(p, cx_realloc);
  if (CX_LIKELY(p))
  {
    void *newp = CX_HDR2PTR(p);
    if (size > old_size)
      memset(newp + old_size, 0, size - old_size);
    CX_HDR(p)->sz = size;
    p = newp;
  }
  return p;
}

void cx_free(void *p)
{
  if (CX_LIKELY(p))
    CX_FREE(CX_PTR2HDR(p));
}

void *cx_memclear(void *p)
{
  if (CX_LIKELY(p))
    memset(p, 0, CX_PTR2HDR(p)->sz);
  return p;
}
