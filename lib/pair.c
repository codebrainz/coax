#include <coax/pair.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int cx_pair_init(cx_pair_t *pr, void *first, void *second)
{
  assert(pr);
  pr->items[CX_PAIR_FIRST_INDEX] = first;
  pr->items[CX_PAIR_SECOND_INDEX] = second;
  return 0;
}

int cx_pair_cleanup(cx_pair_t *pr)
{
  assert(pr);
  if (pr == NULL)
    return -1;
  memset(pr, 0, sizeof(cx_pair_t));
  return 0;
}

cx_pair_t *cx_pair_new(void *first, void *second)
{
  cx_pair_t *pr = calloc(1, sizeof(cx_pair_t));
  if (pr == NULL)
    return NULL;
  if (cx_pair_init(pr, first, second) != 0)
  {
    free(pr);
    pr = NULL;
  }
  return pr;
}

void cx_pair_free(cx_pair_t *pr)
{
  if (pr == NULL)
    return;
  cx_pair_cleanup(pr);
  free(pr);
}
