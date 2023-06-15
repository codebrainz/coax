#ifndef CX_MACROS_H
#define CX_MACROS_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#include <coax/log.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define CX_UNUSED(x) ((void)(x))

#ifdef __GNUC__
#define CX_LIKELY(x) __builtin_expect(!!(x), 1)
#define CX_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define CX_LIKELY(x) (x)
#define CX_UNLIKELY(x) (x)
#endif

#ifndef NDEBUG

#ifdef CX_FATAL_CHECK_ARG
#define CX_CHECK_ARG_FUNC cx_fatal
#else
#define CX_CHECK_ARG_FUNC cx_error
#endif

#define CX_ASSERT(exp)                          \
  do                                            \
  {                                             \
    if (CX_UNLIKELY(!(exp)))                    \
      cx_fatal("assertion '" #exp "' failed!"); \
  } while (0)

#define CX_CHECK_ARG(exp)                                \
  do                                                     \
  {                                                      \
    if (CX_UNLIKELY(!(exp)))                             \
    {                                                    \
      CX_CHECK_ARG_FUNC("assertion '" #exp "' failed!"); \
      return -1;                                         \
    }                                                    \
  } while (0)

#define CX_CHECK_ARG_NO_RETVAL(exp)                      \
  do                                                     \
  {                                                      \
    if (CX_UNLIKELY(!(exp)))                             \
    {                                                    \
      CX_CHECK_ARG_FUNC("assertion '" #exp "' failed!"); \
      return;                                            \
    }                                                    \
  } while (0)

#define CX_CHECK_ARG_RET_VAL(exp, val)                   \
  do                                                     \
  {                                                      \
    if (CX_UNLIKELY(!(exp)))                             \
    {                                                    \
      CX_CHECK_ARG_FUNC("assertion '" #exp "' failed!"); \
      return (val);                                      \
    }                                                    \
  } while (0)

#else

#define CX_ASSERT(exp) \
  do                   \
  {                    \
    (void)(exp);       \
  } while (0)

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

#define CX_CHECK_ARG_RET_VAL(exp, val) \
  do                                   \
  {                                    \
    if (CX_UNLIKELY(!(exp)))           \
      return (val);                    \
  } while (0)

#endif // NDEBUG

#endif // CX_MACROS_H
