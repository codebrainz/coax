#include <coax/log.h>
#include <coax/macros.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static const char *cx_log_level_name(cx_log_level_t level)
{
  switch (level)
  {
  case CX_LOG_DEBUG:
    return "DEBUG";
  case CX_LOG_INFO:
    return "INFO";
  case CX_LOG_WARNING:
    return "WARNING";
  case CX_LOG_ERROR:
    return "ERROR";
  case CX_LOG_FATAL:
    return "FATAL";
  }
  assert(0);
  return "INVALID";
}

static FILE *cx_log_level_file(cx_log_level_t level)
{
  switch (level)
  {
  case CX_LOG_DEBUG:
  case CX_LOG_INFO:
    return stdout;
  case CX_LOG_WARNING:
  case CX_LOG_ERROR:
  case CX_LOG_FATAL:
    return stderr;
  }
  assert(0);
  return stderr;
}

void cx_log(const char *domain, cx_log_level_t level, const char *filename, unsigned int line, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  cx_logv(domain, level, filename, line, fmt, ap);
  va_end(ap);
}

void cx_logv(const char *domain,
             cx_log_level_t level,
             const char *filename,
             unsigned int line,
             const char *fmt,
             va_list ap)
{
  FILE *fp = cx_log_level_file(level);
  if (!domain)
    domain = CX_LOG_DOMAIN;
#ifndef NDEBUG
  fprintf(fp, "%s: %s: %s:%u: ", domain, cx_log_level_name(level), filename, line);
#else
  CX_UNUSED(filename);
  CX_UNUSED(line);
  fprintf(fp, "%s: %s: ", domain, cx_log_level_name(level));
#endif
  vfprintf(fp, fmt, ap);
  fputc('\n', fp);
  fflush(fp);
  if (level == CX_LOG_FATAL)
    exit(EXIT_FAILURE);
}
