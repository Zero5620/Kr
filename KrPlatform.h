#pragma once

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
