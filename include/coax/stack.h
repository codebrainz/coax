#ifndef CX_STACK_H
#define CX_STACK_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <coax/list.h>

typedef struct cx_stack cx_stack_t;

struct cx_stack
{
  cx_list_t base_;
};

#define cx_stack_init(stk) cx_list_init((cx_list_t *)(stk))
#define cx_stack_init_full(stk, free) cx_list_init_full((cx_list_t *)(stk), free)
#define cx_stack_cleanup(stk) cx_list_cleanup((cx_list_t *)(stk))

#define cx_stack_new() ((cx_stack_t *)(cx_list_new()))
#define cx_stack_new_full(free) ((cx_stack_t *)(cx_list_new_full(free)))
#define cx_stack_free(stk) cx_list_free((cx_list_t *)(stk))

#define cx_stack_clear(stk) cx_list_clear((cx_list_t *)(stk))

#define cx_stack_empty(stk) (((cx_list_t *)(stk))->len == 0)

#define cx_stack_push(stk, item) cx_list_push_tail((cx_list_t *)(stk), (void *)(item))
#define cx_stack_pop(stk, do_free) cx_list_pop_tail((cx_list_t *)(stk), do_free)
#define cx_stack_top(stk) (((cx_list_t *)(stk))->tail)

#define cx_stack_foreach(stk, fnc, data) cx_list_foreach((cx_list_t *)(stk), (cx_each_func)(fnc), (void *)(data))

#endif // CX_STACK_H
