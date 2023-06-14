#include <coax/alloc.h>

#include <stdio.h>
#include <string.h>

#define CX_CHECK_OOM(p, func)                                        \
  do                                                                 \
  {                                                                  \
    if (!(p))                                                        \
    {                                                                \
      fprintf(stderr, "error: out of memory calling '%s'\n", #func); \
    }                                                                \
  } while (0)

void *cx_malloc(size_t size)
{
  void *p = CX_MALLOC(size);
#if CX_FATAL_OOM
  CX_CHECK_OOM(p, cx_malloc);
#endif
  return p;
}

void *cx_calloc(size_t n, size_t size)
{
  const size_t total = n * size;
  void *p = CX_MALLOC(total);
#if CX_FATAL_OOM
  CX_CHECK_OOM(p, cx_calloc);
#else
  if (p)
    memset(p, 0, total);
#endif
  return p;
}

void *cx_realloc(void *oldp, size_t size)
{
  void *p = CX_REALLOC(oldp, size);
#if CX_FATAL_OOM
  CX_CHECK_OOM(p, cx_realloc);
#endif
  return p;
}

void cx_free(void *p)
{
  if (p)
    CX_FREE(p);
}
