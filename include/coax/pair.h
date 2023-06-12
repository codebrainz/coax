#ifndef CX_PAIR_H
#define CX_PAIR_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#define CX_PAIR_FIRST_INDEX 0
#define CX_PAIR_SECOND_INDEX 1

typedef struct cx_pair cx_pair_t;

struct cx_pair
{
  void *items[2];
};

int cx_pair_init(cx_pair_t *pr, void *first, void *second);
int cx_pair_cleanup(cx_pair_t *pr);

cx_pair_t *cx_pair_new(void *first, void *second);
void cx_pair_free(cx_pair_t *pr);

#define cx_pair_first(pr) (((cx_pair_t *)(pr))->items[CX_PAIR_FIRST_INDEX])
#define cx_pair_second(pr) (((cx_pair_t *)(pr))->items[CX_PAIR_SECOND_INDEX])

#endif // CX_PAIR_H
