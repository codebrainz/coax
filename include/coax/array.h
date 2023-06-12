#ifndef CX_ARRAY_H
#define CX_ARRAY_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <coax/funcs.h>

#include <stddef.h>

typedef struct cx_array cx_array_t;

struct cx_array
{
  void **items;
  size_t size;
  size_t capacity;
  cx_free_func free;
};

#define CX_ARRAY_INIT ((cx_array_t){NULL, 0, 0, NULL})

int cx_array_init(cx_array_t *arr);
int cx_array_init_full(cx_array_t *arr, size_t reserve, cx_free_func free);
int cx_array_cleanup(cx_array_t *arr);

cx_array_t *cx_array_new(void);
cx_array_t *cx_array_new_full(size_t reserve, cx_free_func free);
void cx_array_free(cx_array_t *arr);

int cx_array_reserve(cx_array_t *arr, size_t reserve);
int cx_array_compact(cx_array_t *arr);
int cx_array_clear(cx_array_t *arr);

int cx_array_insert(cx_array_t *arr, size_t pos, void *item);
int cx_array_prepend(cx_array_t *arr, void *item);
int cx_array_append(cx_array_t *arr, void *item);
int cx_array_remove(cx_array_t *arr, size_t pos);

void *cx_array_index(const cx_array_t *arr, size_t index);

int cx_array_sort(cx_array_t *arr, cx_compare_func cmp);
int cx_array_reverse(cx_array_t *arr);
int cx_array_foreach(const cx_array_t *arr, cx_each_func fnc, void *data);

#endif // CX_ARRAY_H
