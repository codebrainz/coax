#include <coax/dict.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define CX_DICT_INIT_BUCKETS 16
#define CX_DICT_GROW_FACTOR 2.0
#define CX_DICT_SHRINK_FACTOR 0.25

struct cx_dict_entry
{
  void *key;
  void *value;
  cx_dict_t *dict;
};

static void cx_dict_free_bucket(void *bucketp)
{
  cx_list_t *lst = bucketp;
  cx_list_free(lst);
}

static void cx_dict_free_list_item(void *itemp)
{
  struct cx_dict_entry *ent = itemp;
  if (ent->dict)
  {
    if (ent->dict->key_free)
      ent->dict->key_free(ent->key);
    if (ent->dict->value_free)
      ent->dict->value_free(ent->value);
    free(ent);
  }
}

static inline size_t cx_dict_hash(const cx_dict_t *dict, const void *key)
{
  size_t hk = dict->hash(key);
  size_t res = hk % dict->buckets.size;
  assert(res < dict->buckets.size);
  return res;
}

static inline double cx_dict_load_factor(const cx_dict_t *dict)
{
  return (double)dict->len / (double)dict->buckets.size;
}

int cx_dict_init(cx_dict_t *dict, cx_hash_func hash, cx_equal_func equal)
{
  assert(dict);
  assert(hash);
  assert(equal);
  return cx_dict_init_full(dict, hash, equal, NULL, NULL);
}

int cx_dict_init_full(cx_dict_t *dict,
                      cx_hash_func hash,
                      cx_equal_func equal,
                      cx_free_func key_free,
                      cx_free_func value_free)
{
  assert(dict);
  assert(hash);
  assert(equal);

  dict->len = 0;
  dict->hash = hash;
  dict->equal = equal;
  dict->key_free = key_free;
  dict->value_free = value_free;

  if (cx_array_init_full(&dict->buckets, CX_DICT_INIT_BUCKETS, cx_dict_free_bucket) != 0)
    return -1;

  for (size_t i = 0; i < CX_DICT_INIT_BUCKETS; i++)
  {
    cx_list_t *lst = cx_list_new_full(cx_dict_free_list_item);
    if (lst == NULL)
    {
      cx_dict_cleanup(dict);
      return -1;
    }
    if (cx_array_append(&dict->buckets, lst) != 0)
    {
      cx_dict_cleanup(dict);
      return -1;
    }
  }

  return 0;
}

int cx_dict_cleanup(cx_dict_t *dict)
{
  assert(dict);
  cx_dict_clear(dict);
  cx_array_cleanup(&dict->buckets);
  return 0;
}

cx_dict_t *cx_dict_new(cx_hash_func hash, cx_equal_func equal)
{
  assert(hash);
  assert(equal);
  return cx_dict_new_full(hash, equal, NULL, NULL);
}

cx_dict_t *cx_dict_new_full(cx_hash_func hash, cx_equal_func equal, cx_free_func key_free, cx_free_func value_free)
{
  assert(hash);
  assert(equal);
  cx_dict_t *dict = calloc(1, sizeof(cx_dict_t));
  if (dict == NULL)
    return NULL;
  if (cx_dict_init_full(dict, hash, equal, key_free, value_free) != 0)
  {
    free(dict);
    dict = NULL;
  }
  return dict;
}

void cx_dict_free(cx_dict_t *dict)
{
  assert(dict);
  cx_dict_cleanup(dict);
  free(dict);
}

int cx_dict_clear(cx_dict_t *dict)
{
  assert(dict);

  for (size_t i = 0; i < dict->buckets.size; i++)
  {
    cx_list_t *lst = dict->buckets.items[i];
    while (lst->len > 0)
    {
      cx_list_pop_head(lst, true);
      dict->len--;
    }
  }

  return 0;
}

static int cx_dict_rehash(cx_dict_t *dict, size_t new_n_buckets)
{
  assert(dict);

  // create new buckets table
  cx_array_t new_buckets = CX_ARRAY_INIT;
  if (cx_array_init_full(&new_buckets, new_n_buckets, NULL) != 0)
    return -1;

  // setup new buckets table
  for (size_t i = 0; i < new_n_buckets; i++)
  {
    cx_list_t *lst = cx_list_new_full(NULL);
    if (lst == NULL)
    {
      cx_array_cleanup(&new_buckets);
      return -1;
    }
    if (cx_array_append(&new_buckets, lst) != 0)
    {
      cx_array_cleanup(&new_buckets);
      return -1;
    }
  }

  // loop through each entry in each bucket of original table
  for (size_t i = 0; i < dict->buckets.size; i++)
  {
    cx_list_t *lst = dict->buckets.items[i];
    for (cx_list_entry_t *it = lst->head; it != NULL; it = it->next)
    {
      // put the existing entry in the new table
      struct cx_dict_entry *ent = it->data;
      size_t hk = dict->hash(ent->key) % new_buckets.size;
      assert(hk < new_buckets.size);
      cx_list_t *dst_lst = new_buckets.items[hk];
      if (cx_list_push_head(dst_lst, ent) != 0)
      {
        cx_array_cleanup(&new_buckets);
        return -1;
      }
    }
    lst->free = NULL;        // prevent freeing later
  }
  dict->buckets.free = NULL; // prevent freeing later

  // swap in the new table
  cx_array_t old_buckets = CX_ARRAY_INIT;
  memcpy(&old_buckets, &dict->buckets, sizeof(cx_array_t));
  memcpy(&dict->buckets, &new_buckets, sizeof(cx_array_t));

  // add the free handlers into new buckets table
  dict->buckets.free = cx_dict_free_bucket;
  for (size_t i = 0; i < dict->buckets.size; i++)
  {
    cx_list_t *lst = dict->buckets.items[i];
    lst->free = cx_dict_free_list_item;
  }

  // cleanup up the old table (won't free any elements)
  cx_array_cleanup(&old_buckets);

  return 0;
}

static int cx_dict_rehash_if_needed(cx_dict_t *dict)
{
  assert(dict);
  double load_factor = cx_dict_load_factor(dict);
  if (load_factor > CX_DICT_GROW_FACTOR)
    return cx_dict_rehash(dict, dict->buckets.size * 2);
  else if (load_factor < CX_DICT_SHRINK_FACTOR)
    return cx_dict_rehash(dict, dict->buckets.size / 2);
  return 0;
}

int cx_dict_get(const cx_dict_t *dict, const void *key, void **value)
{
  assert(dict);
  assert(key);

  size_t hk = cx_dict_hash(dict, key);
  cx_list_t *lst = dict->buckets.items[hk];

  for (cx_list_entry_t *it = lst->head; it != NULL; it = it->next)
  {
    struct cx_dict_entry *ent = it->data;
    if (dict->equal(key, ent->key))
    {
      if (value != NULL)
        *value = ent->value;
      return 1;
    }
  }

  return 0;
}

bool cx_dict_has(const cx_dict_t *dict, const void *key)
{
  assert(dict);
  assert(key);
  return cx_dict_get(dict, key, NULL) == 1;
}

int cx_dict_set(cx_dict_t *dict, void *key, void *value)
{
  assert(dict);
  assert(key);

  bool replaced = false;
  size_t hk = cx_dict_hash(dict, key);
  cx_list_t *lst = dict->buckets.items[hk];

  for (cx_list_entry_t *it = lst->head; it != NULL; it = it->next)
  {
    struct cx_dict_entry *ent = it->data;
    if (dict->equal(key, ent->key))
    {
      if (dict->key_free)
        dict->key_free(ent->key);
      if (dict->value_free)
        dict->value_free(ent->value);
      ent->key = key;
      ent->value = value;
      replaced = true;
      break;
    }
  }

  if (!replaced)
  {
    struct cx_dict_entry *ent = calloc(1, sizeof(struct cx_dict_entry));
    if (ent == NULL)
      return -1;
    ent->dict = dict;
    ent->key = key;
    ent->value = value;
    if (cx_list_push_head(lst, ent) != 0)
    {
      free(ent);
      return -1;
    }
    dict->len++;
  }

  cx_dict_rehash_if_needed(dict);

  return !!replaced;
}

int cx_dict_del(cx_dict_t *dict, const void *key)
{
  assert(dict);
  assert(key);

  bool deleted = false;
  size_t hk = cx_dict_hash(dict, key);
  cx_list_t *lst = dict->buckets.items[hk];

  for (cx_list_entry_t *it = lst->head; it != NULL; it = it->next)
  {
    struct cx_dict_entry *ent = it->data;
    if (dict->equal(key, ent->key))
    {
      if (cx_list_unlink(lst, it) != 0)
        return -1;
      if (dict->key_free)
        dict->key_free(ent->key);
      if (dict->value_free)
        dict->value_free(ent->value);
      free(it);
      free(ent);
      deleted = true;
      break;
    }
  }

  cx_dict_rehash_if_needed(dict);

  return !!deleted;
}
