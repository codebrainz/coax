#ifndef CX_STRFUNCS_H
#define CX_STRFUNCS_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <coax/array.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

size_t cx_strnlen(const char *s, size_t n);

char *cx_strdup(const char *s);
char *cx_strndup(const char *s, size_t n);

char *cx_strdup_fmt(const char *fmt, ...);
char *cx_strdup_vfmt(const char *fmt, va_list ap);

size_t cx_strhash(const char *s);
size_t cx_strnhash(const char *s, size_t n);

bool cx_strequal(const char *s1, const char *s2);
bool cx_strnequal(const char *s1, const char *s2, size_t n);

char *cx_strtok(char *str, const char *delim, char **saveptr);

int cx_strsplit(const char *str, const char *delim, cx_array_t *result);
int cx_strnsplit(const char *str, size_t len, const char *delim, cx_array_t *result);

char *cx_strup(char *str);
char *cx_strnup(char *str, long long int n);
char *cx_strdown(char *str);
char *cx_strndown(char *str, long long int n);

#endif // CX_STRFUNCS_H
