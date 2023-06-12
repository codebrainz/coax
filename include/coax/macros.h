#ifndef CX_MACROS_H
#define CX_MACROS_H 1

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __GNUC__
#define CX_LIKELY(x) __builtin_expect(!!(x), 1)
#define CX_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define CX_LIKELY(x) (x)
#define CX_UNLIKELY(x) (x)
#endif

#ifndef NDEBUG

#define CX_CHECK_ARG(exp)                                                                   \
  do                                                                                        \
  {                                                                                         \
    if (CX_UNLIKELY(!(exp)))                                                                \
    {                                                                                       \
      fprintf(stderr, "error:  %s:%u: assertion '" #exp "' failed!\n", __FILE__, __LINE__); \
      abort();                                                                              \
      return -1;                                                                            \
    }                                                                                       \
  } while (0)

#define CX_CHECK_ARG_NO_RETVAL(exp)                                                         \
  do                                                                                        \
  {                                                                                         \
    if (CX_UNLIKELY(!(exp)))                                                                \
    {                                                                                       \
      fprintf(stderr, "error:  %s:%u: assertion '" #exp "' failed!\n", __FILE__, __LINE__); \
      abort();                                                                              \
      return;                                                                               \
    }                                                                                       \
  } while (0)

#else

#define CX_CHECK_ARG(exp)    \
  do                         \
  {                          \
    if (CX_UNLIKELY(!(exp))) \
    {                        \
      errno = EINVAL;        \
      return -1;             \
    }                        \
  } while (0)

#define CX_CHECK_ARG_NO_RETVAL(exp) \
  do                                \
  {                                 \
    if (CX_UNLIKELY(!(exp)))        \
    {                               \
      errno = EINVAL;               \
      return;                       \
    }                               \
  } while (0)

#endif // NDEBUG

#endif // CX_MACROS_H
