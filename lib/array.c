#include <coax/array.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int cx_array_init(cx_array_t *arr)
{
  assert(arr);
  return cx_array_init_full(arr, 0, NULL);
}

int cx_array_init_full(cx_array_t *arr, size_t reserve, cx_free_func free)
{
  assert(arr);

  arr->items = NULL;
  arr->size = arr->capacity = 0;
  arr->free = free;

  if (reserve > 0 && cx_array_reserve(arr, reserve) != 0)
    return -1;

  return 0;
}

int cx_array_cleanup(cx_array_t *arr)
{
  assert(arr);
  cx_array_clear(arr);
  free(arr->items);
  return 0;
}

cx_array_t *cx_array_new(void)
{
  return cx_array_new_full(0, NULL);
}

cx_array_t *cx_array_new_full(size_t reserve, cx_free_func free)
{
  cx_array_t *arr = calloc(1, sizeof(cx_array_t));
  if (arr == NULL)
    return NULL;
  if (cx_array_init_full(arr, reserve, free) != 0)
  {
    free(arr);
    arr = NULL;
  }
  return arr;
}

void cx_array_free(cx_array_t *arr)
{
  if (arr == NULL)
    return;
  cx_array_cleanup(arr);
  free(arr);
}

int cx_array_reserve(cx_array_t *arr, size_t reserve)
{
  assert(arr);
  if (reserve > arr->capacity)
  {
    void *tmp = realloc(arr->items, reserve * sizeof(void *));
    if (tmp == NULL)
      return -1;
    arr->items = tmp;
    arr->capacity = reserve;
  }
  return 0;
}

int cx_array_compact(cx_array_t *arr)
{
  assert(arr);
  if (arr->size != arr->capacity)
  {
    void *tmp = realloc(arr->items, arr->size * sizeof(void *));
    if (tmp == NULL)
      return -1;
    arr->items = tmp;
    arr->capacity = arr->size;
  }
  return 0;
}

int cx_array_clear(cx_array_t *arr)
{
  assert(arr);
  if (arr->free)
  {
    for (size_t i = 0; i < arr->size; i++)
      arr->free(arr->items[i]);
  }
  arr->size = 0;
  return 0;
}

static int cx_array_ensure_capacity(cx_array_t *arr, size_t req_cap)
{
  assert(arr);
  if (req_cap > arr->capacity)
  {
    size_t new_cap = arr->capacity * 2;
    if (req_cap > new_cap)
      new_cap = req_cap;
    void *tmp = realloc(arr->items, new_cap * sizeof(void *));
    if (tmp == NULL)
      return -1;
    arr->items = tmp;
    arr->capacity = new_cap;
  }
  else if (req_cap < (arr->capacity / 4))
  {
    size_t new_cap = arr->capacity / 2;
    void *tmp = realloc(arr->items, new_cap * sizeof(void *));
    if (tmp == NULL)
      return -1;
    arr->items = tmp;
    arr->capacity = new_cap;
  }
  return 0;
}

int cx_array_insert(cx_array_t *arr, size_t pos, void *item)
{
  assert(arr);
  assert(pos <= arr->size);

  size_t new_size = arr->size + 1;

  if (cx_array_ensure_capacity(arr, new_size) != 0)
    return -1;

  // move existing items out of the way
  for (long long int i = (long long int)arr->size - 1; i >= (long long int)pos; i--)
    arr->items[i + 1] = arr->items[i];

  arr->items[pos] = item;
  arr->size = new_size;

  return 0;
}

int cx_array_prepend(cx_array_t *arr, void *item)
{
  assert(arr);
  return cx_array_insert(arr, 0, item);
}

int cx_array_append(cx_array_t *arr, void *item)
{
  assert(arr);
  return cx_array_insert(arr, arr->size, item);
}

int cx_array_remove(cx_array_t *arr, size_t pos)
{
  assert(arr);
  assert(pos < arr->size);
  if (arr->free)
    arr->free(arr->items[pos]);
  for (size_t i = pos; i < arr->size; i++)
    arr->items[i] = arr->items[i + 1];
  arr->size--;
  return 0;
}

void *cx_array_index(const cx_array_t *arr, size_t index)
{
  assert(arr);
  assert(index < arr->size);
  return arr->items[index];
}

int cx_array_sort(cx_array_t *arr, cx_compare_func cmp)
{
  assert(arr);
  assert(cmp);
  qsort(arr->items, arr->size, sizeof(void *), cmp);
  return 0;
}

int cx_array_reverse(cx_array_t *arr)
{
  assert(arr);
  for (size_t i = 0; i < arr->size / 2; i++)
  {
    void *tmp = arr->items[i];
    arr->items[i] = arr->items[arr->size - 1 - i];
    arr->items[arr->size - 1 - i] = tmp;
  }
  return 0;
}

int cx_array_foreach(const cx_array_t *arr, cx_each_func fnc, void *data)
{
  assert(arr);
  assert(fnc);
  int cnt = 0;
  for (size_t i = 0; i < arr->size; i++)
  {
    cnt++;
    if (!fnc(arr->items[i], data))
      break;
  }
  return cnt;
}
