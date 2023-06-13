#include <coax/list.h>
#include <coax/macros.h>

#include <stdlib.h>
#include <string.h>

int cx_list_init(cx_list_t *lst)
{
  CX_CHECK_ARG(lst);
  return cx_list_init_full(lst, NULL);
}

int cx_list_init_full(cx_list_t *lst, cx_free_func free_func)
{
  CX_CHECK_ARG(lst);
  memset(lst, 0, sizeof(cx_list_t));
  lst->free = free_func;
  return 0;
}

int cx_list_cleanup(cx_list_t *lst)
{
  CX_CHECK_ARG(lst);
  cx_list_clear(lst);
  memset(lst, 0, sizeof(cx_list_t));
  return 0;
}

cx_list_t *cx_list_new(void)
{
  return cx_list_new_full(NULL);
}

cx_list_t *cx_list_new_full(cx_free_func free_func)
{
  cx_list_t *lst = calloc(1, sizeof(cx_list_t));
  if (lst == NULL)
    return NULL;
  if (cx_list_init_full(lst, free_func) != 0)
  {
    free(lst);
    lst = NULL;
  }
  return lst;
}

void cx_list_free(cx_list_t *lst)
{
  CX_CHECK_ARG_NO_RETVAL(lst);
  cx_list_cleanup(lst);
  free(lst);
}

int cx_list_clear(cx_list_t *lst)
{
  CX_CHECK_ARG(lst);
  cx_list_entry_t *it = lst->head;
  while (it != NULL)
  {
    cx_list_entry_t *next = it->next;
    if (lst->free)
      lst->free(it->data);
    free(it);
    it = next;
  }
  return 0;
}

int cx_list_push_head(cx_list_t *lst, void *data)
{
  CX_CHECK_ARG(lst);
  cx_list_entry_t *ent = calloc(1, sizeof(cx_list_entry_t));
  if (ent == NULL)
    return -1;
  ent->data = data;
  ent->prev = NULL;
  ent->next = lst->head;
  if (lst->head != NULL)
    lst->head->prev = ent;
  lst->head = ent;
  if (lst->tail == NULL)
    lst->tail = lst->head;
  lst->len++;
  return 0;
}

int cx_list_push_tail(cx_list_t *lst, void *data)
{
  CX_CHECK_ARG(lst);
  cx_list_entry_t *ent = calloc(1, sizeof(cx_list_entry_t));
  if (ent == NULL)
    return -1;
  ent->data = data;
  ent->prev = lst->tail;
  ent->next = NULL;
  if (lst->tail != NULL)
    lst->tail->next = ent;
  lst->tail = ent;
  if (lst->head == NULL)
    lst->head = lst->tail;
  lst->len++;
  return 0;
}

int cx_list_pop_head(cx_list_t *lst, bool do_free)
{
  CX_CHECK_ARG(lst);
  cx_list_entry_t *ent = lst->head;
  cx_list_unlink(lst, ent);
  if (do_free && lst->free)
    lst->free(ent->data);
  free(ent);
  return 0;
}

int cx_list_pop_tail(cx_list_t *lst, bool do_free)
{
  CX_CHECK_ARG(lst);
  cx_list_entry_t *ent = lst->tail;
  cx_list_unlink(lst, ent);
  if (do_free && lst->free)
    lst->free(ent->data);
  free(ent);
  return 0;
}

int cx_list_remove_nth(cx_list_t *lst, size_t len, bool do_free)
{
  CX_CHECK_ARG(lst);
  CX_CHECK_ARG(len < lst->len);
  size_t i = 0;
  cx_list_entry_t *it = lst->head;
  while (it != NULL)
  {
    cx_list_entry_t *next = it->next;
    if (i == len)
    {
      cx_list_unlink(lst, it);
      if (do_free && lst->free)
        lst->free(it->data);
      free(it);
      break;
    }
    it = next;
    i++;
  }
  return 0;
}

int cx_list_foreach(cx_list_t *lst, cx_each_func fnc, void *data)
{
  CX_CHECK_ARG(lst);
  CX_CHECK_ARG(fnc);
  int cnt = 0;
  for (cx_list_entry_t *it = lst->head; it != NULL; it = it->next)
  {
    cnt++;
    if (!fnc(it->data, data))
      break;
  }
  return cnt;
}

int cx_list_unlink(cx_list_t *lst, cx_list_entry_t *ent)
{
  CX_CHECK_ARG(lst);
  CX_CHECK_ARG(ent);
  // TODO: write this properly
  if (lst->len == 0)
    return -1;
  if (ent == lst->head)
  {
    lst->head = ent->next;
    if (lst->head != NULL)
      lst->head->prev = NULL;
    else
      lst->tail = NULL;
  }
  else if (ent == lst->tail)
  {
    lst->tail = ent->prev;
    if (lst->tail != NULL)
      lst->tail->next = NULL;
    else
      lst->head = NULL;
  }
  else
  {
    if (ent->prev)
      ent->prev->next = ent->next;
    if (ent->next)
      ent->next->prev = ent->prev;
  }
  lst->len--;
  ent->prev = ent->next = NULL;
  return 0;
}
