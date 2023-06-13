#include <coax/filefuncs.h>
#include <coax/macros.h>
#include <coax/strfuncs.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

char *cx_file_get_contents(const char *fn, size_t *count)
{
  CX_CHECK_ARG_RET_VAL(fn != NULL, NULL);

  FILE *fp = NULL;
  char *buf = NULL;

  fp = fopen(fn, "rb");
  if (fp == NULL)
    goto failure_exit;

  if (fseek(fp, 0, SEEK_END) != 0)
    goto failure_exit;

  long len = ftell(fp);
  if (len < 0)
    goto failure_exit;

  if (fseek(fp, 0, SEEK_SET) != 0)
    goto failure_exit;

  buf = malloc(len + 1);
  if (buf == NULL)
    goto failure_exit;

  size_t nread = fread(buf, 1, len, fp);
  if (ferror(fp) || nread != (size_t)len)
    goto failure_exit;
  buf[len] = '\0';

  if (fclose(fp) != 0)
    goto failure_exit;

  if (count)
    *count = len;

  return buf;

failure_exit : {
  int e = errno;
  if (fp != NULL)
    fclose(fp);
  free(buf);
  errno = e;
  return NULL;
}
}

int cx_file_set_contents(const char *fn, const char *data, size_t count)
{
  CX_CHECK_ARG(fn);
  CX_CHECK_ARG(data);
  CX_CHECK_ARG(count > 0);

  FILE *fp = fopen(fn, "wb");
  if (fp == NULL)
    goto failure_exit;

  size_t nwrite = fwrite(data, 1, count, fp);
  if (ferror(fp) || nwrite != count)
    goto failure_exit;

  return 0;

failure_exit : {
  int e = errno;
  if (fp != NULL)
    fclose(fp);
  errno = e;
  return -1;
}
}

int cx_file_get_contents_str(const char *fn, cx_str_t *contents)
{
  CX_CHECK_ARG(fn);
  CX_CHECK_ARG(contents);

  size_t count = 0;
  char *buf = NULL;

  buf = cx_file_get_contents(fn, &count);
  if (buf == NULL)
    return -1;

  free(contents->data);
  contents->data = buf;
  contents->size = count;
  contents->capacity = count + 1;

  return 0;
}

int cx_file_set_contents_str(const char *fn, const cx_str_t *contents)
{
  CX_CHECK_ARG(fn);
  CX_CHECK_ARG(contents);
  return cx_file_set_contents(fn, contents->data, contents->size);
}

int cx_file_read_lines(const char *fn, cx_array_t *lines)
{
  CX_CHECK_ARG(fn);
  CX_CHECK_ARG(lines);

  cx_str_t contents = CX_STR_INIT;

  if (cx_file_get_contents_str(fn, &contents) != 0)
  {
    cx_str_cleanup(&contents);
    return -1;
  }

  if (cx_strnsplit(contents.data, contents.size, "\n", lines) != 0)
  {
    cx_str_cleanup(&contents);
    return -1;
  }

  if (cx_str_cleanup(&contents) != 0)
    return -1;

  return 0;
}
