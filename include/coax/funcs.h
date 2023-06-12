#ifndef CX_FUNCS_H
#define CX_FUNCS_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <stdbool.h>
#include <stddef.h>

typedef void (*cx_free_func)(void *);
typedef int (*cx_compare_func)(const void *, const void *);
typedef bool (*cx_each_func)(void *, void *);
typedef size_t (*cx_hash_func)(const void *);
typedef bool (*cx_equal_func)(const void *, const void *);

#endif // CX_FUNCS_H
