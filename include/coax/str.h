#ifndef CX_STR_H
#define CX_STR_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct cx_str cx_str_t;

struct cx_str
{
  char *data;
  size_t size;
  size_t capacity;
};

#define CX_STR_INIT ((cx_str_t){NULL, 0, 0})

int cx_str_init(cx_str_t *str);
int cx_str_init_full(cx_str_t *str, size_t reserve);
int cx_str_init_str(cx_str_t *str, const char *s);
int cx_str_init_str_len(cx_str_t *str, const char *s, long long int len);
int cx_str_cleanup(cx_str_t *str);

cx_str_t *cx_str_new(void);
cx_str_t *cx_str_new_full(size_t reserve);
cx_str_t *cx_str_new_str(const char *s);
cx_str_t *cx_str_new_str_len(const char *s, long long int len);
void cx_str_free(cx_str_t *str);

int cx_str_copy(const cx_str_t *src, cx_str_t *dst);
cx_str_t *cx_str_dup(const cx_str_t *src);

int cx_str_reserve(cx_str_t *str, size_t reserve);
int cx_str_compact(cx_str_t *str);
int cx_str_clear(cx_str_t *str);

int cx_str_assign(cx_str_t *str, const char *s);
int cx_str_assign_len(cx_str_t *str, const char *s, long long int len);
int cx_str_assign_ch(cx_str_t *str, int ch);
int cx_str_assign_fmt(cx_str_t *str, const char *fmt, ...);
int cx_str_assign_vfmt(cx_str_t *str, const char *fmt, va_list ap);

int cx_str_insert(cx_str_t *str, size_t pos, const char *s);
int cx_str_insert_len(cx_str_t *str, size_t pos, const char *s, long long int len);
int cx_str_insert_ch(cx_str_t *str, size_t pos, int ch);
int cx_str_insert_fmt(cx_str_t *str, size_t pos, const char *fmt, ...);
int cx_str_insert_vfmt(cx_str_t *str, size_t pos, const char *fmt, va_list ap);

int cx_str_prepend(cx_str_t *str, const char *s);
int cx_str_prepend_len(cx_str_t *str, const char *s, long long int len);
int cx_str_prepend_ch(cx_str_t *str, int ch);
int cx_str_prepend_fmt(cx_str_t *str, const char *fmt, ...);
int cx_str_prepend_vfmt(cx_str_t *str, const char *fmt, va_list ap);

int cx_str_append(cx_str_t *str, const char *s);
int cx_str_append_len(cx_str_t *str, const char *s, long long int len);
int cx_str_append_ch(cx_str_t *str, int ch);
int cx_str_append_fmt(cx_str_t *str, const char *fmt, ...);
int cx_str_append_vfmt(cx_str_t *str, const char *fmt, va_list ap);

int cx_str_remove(cx_str_t *str, size_t pos, size_t len);
int cx_str_remove_ch(cx_str_t *str, size_t pos);

size_t cx_str_hash(const cx_str_t *str);
bool cx_str_equal(const cx_str_t *str1, const cx_str_t *str2);

#endif // CX_STR_H
