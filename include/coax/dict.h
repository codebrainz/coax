#ifndef CX_DICT_H
#define CX_DICT_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <coax/array.h>
#include <coax/funcs.h>
#include <coax/list.h>

#include <stddef.h>

typedef struct cx_dict cx_dict_t;

struct cx_dict
{
  size_t len;
  cx_array_t buckets;
  cx_hash_func hash;
  cx_equal_func equal;
  cx_free_func key_free;
  cx_free_func value_free;
};

#define CX_DICT_INIT ((cx_dict_t){0, CX_PTRARRAY_INIT, NULL, NULL, NULL, NULL})

int cx_dict_init(cx_dict_t *dict, cx_hash_func hash, cx_equal_func equal);
int cx_dict_init_full(cx_dict_t *dict,
                      cx_hash_func hash,
                      cx_equal_func equal,
                      cx_free_func key_free,
                      cx_free_func value_free);
int cx_dict_cleanup(cx_dict_t *dict);

cx_dict_t *cx_dict_new(cx_hash_func hash, cx_equal_func equal);
cx_dict_t *cx_dict_new_full(cx_hash_func hash, cx_equal_func equal, cx_free_func key_free, cx_free_func value_free);
void cx_dict_free(cx_dict_t *dict);

int cx_dict_clear(cx_dict_t *dict);

int cx_dict_get(const cx_dict_t *dict, const void *key, void **value);
bool cx_dict_has(const cx_dict_t *dict, const void *key);
int cx_dict_set(cx_dict_t *dict, void *key, void *value);
int cx_dict_del(cx_dict_t *dict, const void *key);

#endif // CX_DICT_H
