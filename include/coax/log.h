#ifndef CX_LOG_H
#define CX_LOG_H 1

#ifndef CX_LOG_DOMAIN
#define CX_LOG_DOMAIN "coax"
#endif

#include <stdarg.h>

typedef enum cx_log_level
{
  CX_LOG_DEBUG,
  CX_LOG_INFO,
  CX_LOG_WARNING,
  CX_LOG_ERROR,
  CX_LOG_FATAL,
} cx_log_level_t;

void cx_log(const char *domain, cx_log_level_t level, const char *filename, unsigned int line, const char *fmt, ...);

void cx_logv(const char *domain,
             cx_log_level_t level,
             const char *filename,
             unsigned int line,
             const char *fmt,
             va_list ap);

// FIXME: make sure ##__VA_ARGS__ is portable or fix this if not
#define cx_debug(...) cx_log(CX_LOG_DOMAIN, CX_LOG_DEBUG, __FILE__, __LINE__, ##__VA_ARGS__)
#define cx_info(...) cx_log(CX_LOG_DOMAIN, CX_LOG_INFO, __FILE__, __LINE__, ##__VA_ARGS__)
#define cx_warning(...) cx_log(CX_LOG_DOMAIN, CX_LOG_WARNING, __FILE__, __LINE__, ##__VA_ARGS__)
#define cx_error(...) cx_log(CX_LOG_DOMAIN, CX_LOG_ERROR, __FILE__, __LINE__, ##__VA_ARGS__)
#define cx_fatal(...) cx_log(CX_LOG_DOMAIN, CX_LOG_FATAL, __FILE__, __LINE__, ##__VA_ARGS__)

#endif // CX_LOG_H
