#ifndef CX_FILEFUNCS_H
#define CX_FILEFUNCS_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <coax/ptrarray.h>
#include <coax/str.h>

#include <stddef.h>

char *cx_file_get_contents(const char *fn, size_t *count);
int cx_file_set_contents(const char *fn, const char *data, size_t count);

int cx_file_get_contents_str(const char *fn, cx_str_t *contents);
int cx_file_set_contents_str(const char *fn, const cx_str_t *contents);

int cx_file_read_lines(const char *fn, cx_ptrarray_t *lines);

#endif // CX_FILEFUNCS_H
