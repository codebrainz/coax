#ifndef CX_LIST_H
#define CX_LIST_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <coax/funcs.h>

#include <stdbool.h>
#include <stddef.h>

typedef struct cx_list cx_list_t;
typedef struct cx_list_entry cx_list_entry_t;

struct cx_list
{
  size_t len;
  cx_list_entry_t *head;
  cx_list_entry_t *tail;
  cx_free_func free;
};

struct cx_list_entry
{
  void *data;
  cx_list_entry_t *prev;
  cx_list_entry_t *next;
};

#define CX_LIST_INIT ((cx_list_t){0, NULL, NULL, NULL})

int cx_list_init(cx_list_t *lst);
int cx_list_init_full(cx_list_t *lst, cx_free_func free_func);
int cx_list_cleanup(cx_list_t *lst);
cx_list_t *cx_list_new(void);
cx_list_t *cx_list_new_full(cx_free_func free_func);
void cx_list_free(cx_list_t *lst);
int cx_list_clear(cx_list_t *lst);
int cx_list_push_head(cx_list_t *lst, void *data);
int cx_list_push_tail(cx_list_t *lst, void *data);
int cx_list_pop_head(cx_list_t *lst, bool do_free);
int cx_list_pop_tail(cx_list_t *lst, bool do_free);
int cx_list_remove_nth(cx_list_t *lst, size_t len, bool do_free);
int cx_list_foreach(cx_list_t *lst, cx_each_func fnc, void *data);

int cx_list_unlink(cx_list_t *lst, cx_list_entry_t *ent);

#endif // CX_LIST_H
