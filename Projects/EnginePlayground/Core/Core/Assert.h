#pragma once

#include "Core/Config.h"

#if defined(__has_builtin)
  #if __has_builtin(__builtin_debugtrap)
    #define coreDebugBreak() __builtin_debugtrap()
  #elif __has_builtin(__debugbreak)
    #define coreDebugBreak() __debugbreak()
  #endif
#endif

#if !defined(coreDebugBreak)
  #if defined(_MSC_VER)
    #define coreDebugBreak() __debugbreak()
  #elif defined(__i386__) || defined(__x86_64__)
static inline void coreDebugBreak(void)
{
  __asm__ __volatile__("int $03");
}
  #elif defined(__aarch64__)
static inline void coreDebugBreak(void)
{
  __asm__ __volatile__(".inst 0xd4200000");
}
  #elif defined(__arm__)
static inline void coreDebugBreak(void)
{
  __asm__ __volatile__(".inst 0xe7f001f0");
}
  #elif __has_include(signal.h)
    #include <signal.h>
    #if defined(SIGTRAP)
      #define psnip_trap() raise(SIGTRAP)
    #else
      #define psnip_trap() raise(SIGABRT)
    #endif
  #else
    #error No support for coreDebugBreak found.
  #endif
#endif

#ifdef _DEBUG
  #ifndef CORE_ASSERT_ENABLED
    #define CORE_ASSERT_ENABLED 1
  #endif // !CORE_ASSERT_ENABLED
#endif

#if CORE_ASSERT_ENABLED
  #define CORE_ASSERT(expr, ...) \
    do                           \
    {                            \
      if (!(expr))               \
        [[unlikely]]             \
        {                        \
          coreDebugBreak();      \
        }                        \
    } while (0)
#else
  #define CORE_ASSERT(expr, ...)
#endif // CORE_ASSERT_ENABLE
