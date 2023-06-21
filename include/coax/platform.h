#ifndef CX_PLATFORM_H
#define CX_PLATFORM_H 1

#if !defined(CX_INSIDE_HEADER) && !defined(CX_INSIDE_COMPILATION)
#error "Only <coax.h> can be included directly."
#endif

#if defined(__APPLE__) && defined(__MACH__)
#define CX_PLAT_MACOS 1
#endif

#if defined(__linux__) || defined(__gnu_linux__)
#define CX_PLAT_LINUX 1
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define CX_PLAT_MINGW 1
#ifdef __MINGW32__
#define CX_PLAT_MINGW32 1
#endif
#ifdef __MINGW64__
#define CX_PLAT_MINGW64 1
#endif
#endif

#if defined(__unix__)
#define CX_PLAT_UNIX 1
#endif

#if defined(_WIN32) || defined(CX_PLAT_MINGW)
#define CX_PLAT_WIN32 1
#include <windows.h>
#endif

#if defined(CX_PLAT_UNIX) || defined(CX_PLAT_LINUX) || defined(CX_PLAT_MACOS)
#define CX_PLAT_POSIX 1
#include <unistd.h>
#endif

#if !defined(CX_PLAT_WIN32) && !defined(CX_PLAT_POSIX)
#error "unsupported platform"
#endif

#endif // CX_PLATFORM_H
