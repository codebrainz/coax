#ifndef CX_PTRARRAY_H
#define CX_PTRARRAY_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <coax/funcs.h>

#include <stddef.h>

typedef struct cx_ptrarray cx_ptrarray_t;

struct cx_ptrarray
{
  void **items;
  size_t size;
  size_t capacity;
  cx_free_func free;
};

#define CX_PTRARRAY_INIT ((cx_ptrarray_t){NULL, 0, 0, NULL})

int cx_ptrarray_init(cx_ptrarray_t *arr);
int cx_ptrarray_init_full(cx_ptrarray_t *arr, size_t reserve, cx_free_func free);
int cx_ptrarray_cleanup(cx_ptrarray_t *arr);

cx_ptrarray_t *cx_ptrarray_new(void);
cx_ptrarray_t *cx_ptrarray_new_full(size_t reserve, cx_free_func free);
void cx_ptrarray_free(cx_ptrarray_t *arr);

int cx_ptrarray_reserve(cx_ptrarray_t *arr, size_t reserve);
int cx_ptrarray_compact(cx_ptrarray_t *arr);
int cx_ptrarray_clear(cx_ptrarray_t *arr);

int cx_ptrarray_insert(cx_ptrarray_t *arr, size_t pos, void *item);
int cx_ptrarray_prepend(cx_ptrarray_t *arr, void *item);
int cx_ptrarray_append(cx_ptrarray_t *arr, void *item);
int cx_ptrarray_remove(cx_ptrarray_t *arr, size_t pos);

void *cx_ptrarray_index(const cx_ptrarray_t *arr, size_t index);

int cx_ptrarray_sort(cx_ptrarray_t *arr, cx_compare_func cmp);
int cx_ptrarray_reverse(cx_ptrarray_t *arr);
int cx_ptrarray_foreach(const cx_ptrarray_t *arr, cx_each_func fnc, void *data);

#endif // CX_PTRARRAY_H
