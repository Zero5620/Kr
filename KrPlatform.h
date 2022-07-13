#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

#if defined(__clang__) || defined(__ibmxl__)
#define COMPILER_CLANG 1
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#elif defined(__GNUC__)
#define COMPILER_GCC 1
#elif defined(__MINGW32__) || defined(__MINGW64__)
#define COMPILER_MINGW 1
#elif defined(__INTEL_COMPILER)
#define COMPILER_INTEL 1
#else
#error Missing Compiler detection
#endif

#if !defined(COMPILER_CLANG)
#define COMPILER_CLANG 0
#endif
#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif
#if !defined(COMPILER_GCC)
#define COMPILER_GCC 0
#endif
#if !defined(COMPILER_INTEL)
#define COMPILER_INTEL 0
#endif

#if defined(__ANDROID__) || defined(__ANDROID_API__)
#define PLATFORM_ANDROID 1
#define __PLATFORM__ "Andriod"
#elif defined(__gnu_linux__) || defined(__linux__) || defined(linux) || defined(__linux)
#define PLATFORM_LINUX 1
#define __PLATFORM__ "linux"
#elif defined(macintosh) || defined(Macintosh)
#define PLATFORM_MAC 1
#define __PLATFORM__ "Mac"
#elif defined(__APPLE__) && defined(__MACH__)
#define PLATFORM_MAC 1
#define __PLATFORM__ "Mac"
#elif defined(__APPLE__)
#define PLATFORM_IOS 1
#define __PLATFORM__ "iOS"
#elif defined(_WIN64) || defined(_WIN32)
#define PLATFORM_WINDOWS 1
#define __PLATFORM__ "Windows"
#else
#error Missing Operating System Detection
#endif

#if !defined(PLATFORM_ANDRIOD)
#define PLATFORM_ANDRIOD 0
#endif
#if !defined(PLATFORM_LINUX)
#define PLATFORM_LINUX 0
#endif
#if !defined(PLATFORM_MAC)
#define PLATFORM_MAC 0
#endif
#if !defined(PLATFORM_IOS)
#define PLATFORM_IOS 0
#endif
#if !defined(PLATFORM_WINDOWS)
#define PLATFORM_WINDOWS 0
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_AMD64) || \
    defined(_M_X64)
#define ARCH_X64 1
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86) || defined(_X86_)
#define ARCH_X86 1
#elif defined(__arm__) || defined(__thumb__) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT)
#define ARCH_ARM 1
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#else
#error Missing Architecture Identification
#endif

#if !defined(ARCH_X64)
#define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
#define ARCH_X86 0
#endif
#if !defined(ARCH_ARM)
#define ARCH_ARM 0
#endif
#if !defined(ARCH_ARM64)
#define ARCH_ARM64 0
#endif

#if defined(__GNUC__)
#define __PROCEDURE__ __FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define __PROCEDURE__ __PRETTY_PROCEDURE__
#elif defined(__FUNCSIG__)
#define __PROCEDURE__ __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define __PROCEDURE__ __PROCEDURE__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define __PROCEDURE__ __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define __PROCEDURE__ __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define __PROCEDURE__ __func__
#elif defined(_MSC_VER)
#define __PROCEDURE__ __FUNCSIG__
#else
#define __PROCEDURE__ "_unknown_"
#endif

#if defined(HAVE_SIGNAL_H) && !defined(__WATCOMC__)
#include <signal.h> // raise()
#endif

#if defined(_MSC_VER)
#define TriggerBreakpoint() __debugbreak()
#elif ((!defined(__NACL__)) && \
       ((defined(__GNUC__) || defined(__clang__)) && (defined(__i386__) || defined(__x86_64__))))
#define TriggerBreakpoint() __asm__ __volatile__("int $3\n\t")
#elif defined(__386__) && defined(__WATCOMC__)
#define TriggerBreakpoint() _asm { int 0x03}
#elif defined(HAVE_SIGNAL_H) && !defined(__WATCOMC__)
#define TriggerBreakpoint() raise(SIGTRAP)
#else
#define TriggerBreakpoint() ((int *)0) = 0
#endif

#if defined(COMPILER_GCC)
#define inproc static inline
#else
#define inproc inline
#endif

#if !defined(BUILD_DEBUG) && !defined(BUILD_DEVELOPER) && !defined(BUILD_RELEASE)
#if defined(_DEBUG) || defined(DEBUG)
#define BUILD_DEBUG
#elif defined(NDEBUG)
#define BUILD_RELEASE
#else
#define BUILD_DEBUG
#endif
#endif

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
#define DebugTriggerbreakpoint TriggerBreakpoint
#else
#define DebugTriggerbreakpoint()
#endif

inproc void Unimplemented() { TriggerBreakpoint(); }

#ifdef __GNUC__
[[noreturn]] inproc __attribute__((always_inline)) void Unreachable() { DebugTriggerbreakpoint(); __builtin_unreachable(); }
#elif defined(_MSC_VER)
[[noreturn]] __forceinline void Unreachable() { DebugTriggerbreakpoint(); __assume(false); }
#else // ???
inproc void Unreachable() { TriggerBreakpoint(); }
#endif

#define NoDefaultCase() default: Unreachable(); break

//
//
//

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))
#define Minimum(a, b) (((a) < (b)) ? (a) : (b))
#define Maximum(a, b) (((a) > (b)) ? (a) : (b))
#define Clamp(a, b, v) Minimum(b, Maximum(a, v))

#define SetFlag(expr, flag) ((expr) |= (flag))
#define ClearFlag(expr, flag) ((expr) &= ~(flag))
#define ToggleFlag(expr, flag) ((expr) ^= (flag))
#define IsPower2(value) (((value) != 0) && ((value) & ((value)-1)) == 0)
#define AlignPower2Up(x, p) (((x) + (p)-1) & ~((p)-1))
#define AlignPower2Down(x, p) ((x) & ~((p)-1))

#define ByteSwap16(a) ((((a)&0x00FF) << 8) | (((a)&0xFF00) >> 8))
#define ByteSwap32(a) \
    ((((a)&0x000000FF) << 24) | (((a)&0x0000FF00) << 8) | (((a)&0x00FF0000) >> 8) | (((a)&0xFF000000) >> 24))
#define ByteSwap64(a)                                                                                                  \
    ((((a)&0x00000000000000FFULL) << 56) | (((a)&0x000000000000FF00ULL) << 40) | (((a)&0x0000000000FF0000ULL) << 24) | \
     (((a)&0x00000000FF000000ULL) << 8) | (((a)&0x000000FF00000000ULL) >> 8) | (((a)&0x0000FF0000000000ULL) >> 24) |   \
     (((a)&0x00FF000000000000ULL) >> 40) | (((a)&0xFF00000000000000ULL) >> 56))

#define KiloBytes(n) ((n)*1024u)
#define MegaBytes(n) (KiloBytes(n) * 1024u)
#define GigaBytes(n) (MegaBytes(n) * 1024u)
