#ifndef CX_ALLOC_H
#define CX_ALLOC_H 1

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
void cx_free(void *p);

#endif // CX_ALLOC_H
