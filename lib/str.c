#include <coax/macros.h>
#include <coax/str.h>
#include <coax/strfuncs.h>

#include <stdlib.h>
#include <string.h>

int cx_str_init(cx_str_t *str)
{
  CX_CHECK_ARG(str);
  return cx_str_init_full(str, 0);
}

int cx_str_init_full(cx_str_t *str, size_t reserve)
{
  CX_CHECK_ARG(str);
  str->data = NULL;
  str->size = str->capacity = 0;
  if (cx_str_reserve(str, reserve) != 0)
    return -1;
  return 0;
}

int cx_str_init_str(cx_str_t *str, const char *s)
{
  CX_CHECK_ARG(str);
  return cx_str_init_str_len(str, s, -1);
}

int cx_str_init_str_len(cx_str_t *str, const char *s, long long int len)
{
  CX_CHECK_ARG(str);
  if (s == NULL)
    s = "";
  if (len < 0)
    len = strlen(s);
  if (cx_str_init_full(str, len) != 0)
    return -1;
  if (cx_str_assign_len(str, s, len) != 0)
    return -1;
  return 0;
}

int cx_str_cleanup(cx_str_t *str)
{
  CX_CHECK_ARG(str);
  free(str->data);
  return 0;
}

cx_str_t *cx_str_new(void)
{
  return cx_str_new_full(0);
}

cx_str_t *cx_str_new_full(size_t reserve)
{
  cx_str_t *str = calloc(1, sizeof(cx_str_t));
  if (str == NULL)
    return NULL;
  if (cx_str_init_full(str, reserve) == 0)
  {
    free(str);
    str = NULL;
  }
  return str;
}

cx_str_t *cx_str_new_str(const char *s)
{
  return cx_str_new_str_len(s, -1);
}

cx_str_t *cx_str_new_str_len(const char *s, long long int len)
{
  cx_str_t *str = calloc(1, sizeof(cx_str_t));
  if (str == NULL)
    return NULL;
  if (cx_str_init_str_len(str, s, len) != 0)
  {
    free(str);
    str = NULL;
  }
  return str;
}

void cx_str_free(cx_str_t *str)
{
  if (str == NULL)
    return;
  cx_str_cleanup(str);
  free(str);
}

int cx_str_reserve(cx_str_t *str, size_t reserve)
{
  CX_CHECK_ARG(str);
  reserve++;
  if (reserve > str->capacity)
  {
    void *tmp = realloc(str->data, reserve);
    if (tmp == NULL)
      return -1;
    str->data = tmp;
    str->capacity = reserve;
  }
  return 0;
}

int cx_str_compact(cx_str_t *str)
{
  CX_CHECK_ARG(str);
  size_t min_cap = str->size + 1;
  if (min_cap != str->capacity)
  {
    void *tmp = realloc(str->data, min_cap);
    if (tmp == NULL)
      return -1;
    str->data = tmp;
    str->capacity = min_cap;
  }
  return 0;
}

int cx_str_clear(cx_str_t *str)
{
  CX_CHECK_ARG(str);
  str->size = 0;
  if (str->data)
    str->data[0] = '\0';
  return 0;
}

static int cx_str_ensure_capacity(cx_str_t *str, size_t req_cap)
{
  CX_CHECK_ARG(str);
  req_cap++;
  if (req_cap > str->capacity)
  {
    size_t new_cap = str->capacity * 2;
    if (req_cap > new_cap)
      new_cap = req_cap;
    void *tmp = realloc(str->data, new_cap);
    if (tmp == NULL)
      return -1;
    str->data = tmp;
    str->capacity = new_cap;
  }
  else if (req_cap < (str->capacity / 4))
  {
    size_t new_cap = str->capacity / 2;
    void *tmp = realloc(str->data, new_cap);
    if (tmp == NULL)
      return -1;
    str->data = tmp;
    str->capacity = new_cap;
  }
  return 0;
}

int cx_str_assign(cx_str_t *str, const char *s)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(s);
  return cx_str_assign_len(str, s, -1);
}

int cx_str_assign_len(cx_str_t *str, const char *s, long long int len)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(s);
  CX_CHECK_ARG(len >= -1);
  if (cx_str_clear(str) != 0)
    return -1;
  return cx_str_append_len(str, s, len);
}

int cx_str_assign_ch(cx_str_t *str, int ch)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(ch >= 0 && ch <= 255);
  char buf[2] = {ch, '\0'};
  return cx_str_assign_len(str, buf, 1);
}

int cx_str_assign_fmt(cx_str_t *str, const char *fmt, ...)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(fmt);
  va_list ap;
  va_start(ap, fmt);
  int r = cx_str_assign_vfmt(str, fmt, ap);
  va_end(ap);
  return r;
}

int cx_str_assign_vfmt(cx_str_t *str, const char *fmt, va_list ap)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(fmt);
  char *buf = cx_strdup_vfmt(fmt, ap);
  if (buf == NULL)
    return -1;
  int r = cx_str_assign(str, buf);
  free(buf);
  return r;
}

int cx_str_insert(cx_str_t *str, size_t pos, const char *s)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(pos <= str->size);
  CX_CHECK_ARG(s);
  return cx_str_insert_len(str, pos, s, -1);
}

int cx_str_insert_len(cx_str_t *str, size_t pos, const char *s, long long int len)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(pos <= str->size);
  CX_CHECK_ARG(s);
  CX_CHECK_ARG(len >= -1);

  if (len < 0)
    len = strlen(s);

  size_t new_size = str->size + len;

  if (cx_str_ensure_capacity(str, new_size) != 0)
    return -1;

  if (pos < str->size)
    memmove(&str->data[pos + len], &str->data[pos], str->size - pos);

  memcpy(&str->data[pos], s, len);

  str->size += len;
  str->data[str->size] = '\0';

  return 0;
}

int cx_str_insert_ch(cx_str_t *str, size_t pos, int ch)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(pos <= str->size);
  CX_CHECK_ARG(ch >= 0 && ch <= 255);
  char buf[2] = {ch, '\0'};
  return cx_str_insert_len(str, pos, buf, 1);
}

int cx_str_insert_fmt(cx_str_t *str, size_t pos, const char *fmt, ...)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(fmt);
  va_list ap;
  va_start(ap, fmt);
  int r = cx_str_insert_vfmt(str, pos, fmt, ap);
  va_end(ap);
  return r;
}

int cx_str_insert_vfmt(cx_str_t *str, size_t pos, const char *fmt, va_list ap)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(fmt);
  char *buf = cx_strdup_vfmt(fmt, ap);
  if (buf == NULL)
    return -1;
  int r = cx_str_insert(str, pos, buf);
  free(buf);
  return r;
}

int cx_str_prepend(cx_str_t *str, const char *s)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(s);
  return cx_str_prepend_len(str, s, -1);
}

int cx_str_prepend_len(cx_str_t *str, const char *s, long long int len)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(s);
  CX_CHECK_ARG(len >= -1);
  return cx_str_insert_len(str, 0, s, len);
}

int cx_str_prepend_ch(cx_str_t *str, int ch)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(ch >= 0 && ch <= 255);
  char buf[2] = {ch, '\0'};
  return cx_str_prepend_len(str, buf, 1);
}

int cx_str_prepend_fmt(cx_str_t *str, const char *fmt, ...)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(fmt);
  va_list ap;
  va_start(ap, fmt);
  int r = cx_str_prepend_vfmt(str, fmt, ap);
  va_end(ap);
  return r;
}

int cx_str_prepend_vfmt(cx_str_t *str, const char *fmt, va_list ap)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(fmt);
  char *buf = cx_strdup_vfmt(fmt, ap);
  if (buf == NULL)
    return -1;
  int r = cx_str_prepend(str, buf);
  free(buf);
  return r;
}

int cx_str_append(cx_str_t *str, const char *s)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(s);
  return cx_str_append_len(str, s, -1);
}

int cx_str_append_len(cx_str_t *str, const char *s, long long int len)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(s);
  CX_CHECK_ARG(len >= -1);
  return cx_str_insert_len(str, str->size, s, len);
}

int cx_str_append_ch(cx_str_t *str, int ch)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(ch >= 0 && ch <= 255);
  char buf[2] = {ch, '\0'};
  return cx_str_append_len(str, buf, 1);
}

int cx_str_append_fmt(cx_str_t *str, const char *fmt, ...)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(fmt);
  va_list ap;
  va_start(ap, fmt);
  int r = cx_str_append_vfmt(str, fmt, ap);
  va_end(ap);
  return r;
}

int cx_str_append_vfmt(cx_str_t *str, const char *fmt, va_list ap)
{
  CX_CHECK_ARG(str);
  CX_CHECK_ARG(fmt);
  char *buf = cx_strdup_vfmt(fmt, ap);
  if (buf == NULL)
    return -1;
  int r = cx_str_append(str, buf);
  free(buf);
  return r;
}

int cx_str_remove(cx_str_t *str, size_t pos, size_t len)
{
  (void)str;
  (void)pos;
  (void)len;
  return -1;
}

int cx_str_remove_ch(cx_str_t *str, size_t pos)
{
  CX_CHECK_ARG(str);
  return cx_str_remove(str, pos, 1);
}

size_t cx_str_hash(const cx_str_t *str)
{
  CX_CHECK_ARG(str);
  return cx_strnhash(str->data, str->size);
}

bool cx_str_equal(const cx_str_t *str1, const cx_str_t *str2)
{
  CX_CHECK_ARG(str1);
  CX_CHECK_ARG(str2);
  if (str1->size != str2->size)
    return false;
  return cx_strnequal(str1->data, str2->data, str1->size);
}
