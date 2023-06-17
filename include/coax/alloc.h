#ifndef CX_ALLOC_H
#define CX_ALLOC_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <stdlib.h>

// customization points for underlying allocator
#ifndef CX_MALLOC
#define CX_MALLOC malloc
#endif
#ifndef CX_REALLOC
#define CX_REALLOC realloc
#endif
#ifndef CX_FREE
#define CX_FREE free
#endif

#ifndef NDEBUG
#define CX_FATAL_OOM 1
#endif

void *cx_malloc(size_t size);
void *cx_calloc(size_t n, size_t size);
void *cx_realloc(void *oldp, size_t size);
void *cx_crealloc(void *oldp, size_t size);
void cx_free(void *p);
void *cx_memclear(void *p);

#define cx_new_n(T, n) cx_calloc(n, sizeof(T))
#define cx_new(T) cx_new_n(T, 1)

#endif // CX_ALLOC_H
