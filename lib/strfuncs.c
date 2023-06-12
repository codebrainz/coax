#include <coax/strfuncs.h>

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t cx_strnlen(const char *s, size_t n)
{
  const char *e = s;
  while (n-- && *e)
    ++e;
  return (size_t)(e - s);
}

char *cx_strdup(const char *s)
{
  size_t len = strlen(s);
  char *ns = malloc(len + 1);
  if (ns == NULL)
    return NULL;
  memcpy(ns, s, len + 1);
  return ns;
}

char *cx_strndup(const char *s, size_t n)
{
  size_t len = cx_strnlen(s, n);
  char *ns = malloc(len + 1);
  if (ns == NULL)
    return NULL;
  strncpy(ns, s, len);
  ns[len] = '\0';
  return ns;
}

char *cx_strdup_fmt(const char *fmt, ...)
{
  char *r;
  va_list ap;
  va_start(ap, fmt);
  r = cx_strdup_vfmt(fmt, ap);
  va_end(ap);
  return r;
}

char *cx_strdup_vfmt(const char *fmt, va_list ap)
{
  int n = 0;
  size_t size = 0;
  char *p = NULL;
  va_list ap2;

  va_copy(ap2, ap);
  n = vsnprintf(p, size, fmt, ap2);
  va_end(ap2);

  if (n < 0)
    return NULL;

  size = (size_t)n + 1;
  p = malloc(size);
  if (p == NULL)
    return NULL;

  va_copy(ap2, ap);
  n = vsnprintf(p, size, fmt, ap2);
  va_end(ap2);

  if (n < 0)
  {
    free(p);
    return NULL;
  }

  return p;
}

size_t cx_strhash(const char *s)
{
  assert(s != NULL);
  return cx_strnhash(s, strlen(s));
}

size_t cx_strnhash(const char *s, size_t n)
{
  assert(s != NULL);
  size_t hash = 0;
  for (size_t i = 0; i < n; i++)
    hash = s[i] + (hash << 6) + (hash << 16) - hash;
  return hash;
}

bool cx_strequal(const char *s1, const char *s2)
{
  assert(s1 != NULL);
  assert(s2 != NULL);
  if (!s1 && !s2)
    return true;
  else if ((!s1 && s2) || (s1 && !s2))
    return false;
  return strcmp(s1, s2) == 0;
}

bool cx_strnequal(const char *s1, const char *s2, size_t n)
{
  assert(s1 != NULL);
  assert(s2 != NULL);
  if (!s1 && !s2)
    return true;
  else if ((!s1 && s2) || (s1 && !s2))
    return false;
  return strncmp(s1, s2, n);
}

char *cx_strtok(char *str, const char *delim, char **saveptr)
{
  return strtok_r(str, delim, saveptr);
}

int cx_strsplit(const char *str, const char *delim, cx_ptrarray_t *result)
{
  assert(str);
  assert(result);
  return cx_strnsplit(str, strlen(str), delim, result);
}

int cx_strnsplit(const char *str, size_t len, const char *delim, cx_ptrarray_t *result)
{
  assert(str);
  assert(result);

  char *copy = cx_strndup(str, len);
  if (copy == NULL)
    return -1;

  result->free = free;
  result->size = 0;
  if (result->items)
    result->items[0] = NULL;

  char *saveptr = NULL;
  char *token = cx_strtok(copy, delim, &saveptr);

  while (token != NULL)
  {
    char *token_copy = cx_strdup(token);

    if (token_copy == NULL)
    {
      free(copy);
      return -1;
    }

    if (cx_ptrarray_append(result, token_copy) != 0)
    {
      free(token_copy);
      free(copy);
      return -1;
    }

    token = cx_strtok(NULL, delim, &saveptr);
  }

  free(copy);

  if (cx_ptrarray_reserve(result, result->size + 1) != 0)
    return -1;

  result->items[result->size] = NULL;

  return 0;
}

char *cx_strup(char *str)
{
  return cx_strnup(str, -1);
}

char *cx_strnup(char *str, long long int n)
{
  if (str == NULL)
    str = "";
  if (n < 0)
    n = strlen(str);
  for (long long int i = 0; i < n; i++)
    str[i] = toupper(str[i]);
  return str;
}

char *cx_strdown(char *str)
{
  return cx_strndown(str, -1);
}

char *cx_strndown(char *str, long long int n)
{
  if (str == NULL)
    str = "";
  if (n < 0)
    n = strlen(str);
  for (long long int i = 0; i < n; i++)
    str[i] = tolower(str[i]);
  return str;
}
