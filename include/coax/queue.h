#ifndef CX_QUEUE_H
#define CX_QUEUE_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <coax/list.h>

typedef struct cx_queue cx_queue_t;

struct cx_queue
{
  cx_list_t base_;
};

#define cx_queue_init(q) cx_list_init((cx_list_t *)(q))
#define cx_queue_init_full(q, free) cx_list_init_full((cx_list_t *)(q), free)
#define cx_queue_cleanup(q) cx_list_cleanup((cx_list_t *)(q))

#define cx_queue_new() ((cx_queue_t *)(cx_list_new()))
#define cx_queue_new_full(free) ((cx_queue_t *)(cx_list_new_full(free)))
#define cx_queue_free(q) cx_list_free((cx_list_t *)(q))

#define cx_queue_clear(q) cx_list_clear((cx_list_t *)(q))

#define cx_queue_empty(q) (((cx_list_t *)(q))->len == 0)

#define cx_queue_push(q, item) cx_list_push_tail((cx_list_t *)(q), (void *)(item))
#define cx_queue_pop(q, do_free) cx_list_pop_head((cx_list_t *)(q), do_free)

#define cx_queue_tail(q) (((cx_list_t *)(q))->tail)
#define cx_queue_head(q) (((cx_list_t *)(q))->head)

#define cx_queue_foreach(q, fnc, data) cx_list_foreach((cx_list_t *)(q), (cx_each_func)(fnc), (void *)(data))

#endif // CX_QUEUE_H
