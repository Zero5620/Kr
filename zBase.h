#pragma once

#include <stdarg.h>
#include <stdint.h>
#if !defined(__cplusplus)
#include <stdbool.h>
#endif

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
#define OS_ANDROID 1
#elif defined(__gnu_linux__) || defined(__linux__) || defined(linux) || defined(__linux)
#define OS_LINUX 1
#elif defined(macintosh) || defined(Macintosh)
#define OS_MAC 1
#elif defined(__APPLE__) && defined(__MACH__)
#defined OS_MAC 1
#elif defined(__APPLE__)
#define OS_IOS 1
#elif defined(_WIN64) || defined(_WIN32)
#define OS_WINDOWS 1
#else
#error Missing Operating System Detection
#endif

#if !defined(OS_ANDRIOD)
#define OS_ANDRIOD 0
#endif
#if !defined(OS_LINUX)
#define OS_LINUX 0
#endif
#if !defined(OS_MAC)
#define OS_MAC 0
#endif
#if !defined(OS_IOS)
#define OS_IOS 0
#endif
#if !defined(OS_WINDOWS)
#define OS_WINDOWS 0
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_AMD64) || defined(_M_X64)
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

#if !defined(__cplusplus)
#if COMPILER_MSVC == 1
#define thread_local __declspec(thread)
#else
#define thread_local _Thread_local
#endif
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
#include <signal.h>  // raise()
#endif

#if defined(_MSC_VER)
#define TriggerBreakpoint() __debugbreak()
#elif ((!defined(__NACL__)) && ((defined(__GNUC__) || defined(__clang__)) && (defined(__i386__) || defined(__x86_64__))))
#define TriggerBreakpoint() __asm__ __volatile__("int $3\n\t")
#elif defined(__386__) && defined(__WATCOMC__)
#define TriggerBreakpoint() _asm { int 0x03}
#elif defined(HAVE_SIGNAL_H) && !defined(__WATCOMC__)
#define TriggerBreakpoint() raise(SIGTRAP)
#else
#define TriggerBreakpoint() ((int *)0) = 0
#endif

#define Unimplemented() TriggerBreakpoint()
#define Unreachable()   TriggerBreakpoint()
#define NoDefaultCase()      \
default:                 \
TriggerBreakpoint(); \
break

#if !defined(BUILD_DEBUG) && !defined(BUILD_DEVELOPER) && !defined(BUILD_RELEASE)
#if defined(_DEBUG) || defined(DEBUG)
#define BUILD_DEBUG
#elif defined(NDEBUG)
#define BUILD_RELEASE
#else
#define BUILD_DEBUG
#endif
#endif

#if !defined(ASSERTION_HANDLED)
#define AssertHandle(file, line, proc) TriggerBreakpoint()
#else
void AssertHandle(const char *file, int line, const char *proc);
#endif

#if defined(BUILD_DEBUG)
#define DebugTriggerbreakpoint TriggerBreakpoint
#define Assert(x)                                                  \
do {                                                           \
if (!(x)) AssertHandle(__FILE__, __LINE__, __PROCEDURE__); \
} while (0)
#else
#define DebugTriggerbreakpoint()
#define Assert(x) \
do {          \
0;        \
} while (0)
#endif

//
//
//

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;
typedef float    real32;
typedef double   real64;
typedef size_t   ptrsize;

//
//
//

#define _zConcatInternal(x, y) x##y
#define _zConcat(x, y)         _zConcatInternal(x, y)

//
//
//

#define ArrayCount(a)  (sizeof(a) / sizeof((a)[0]))
#define Minimum(a, b)  (((a) < (b)) ? (a) : (b))
#define Maximum(a, b)  (((a) > (b)) ? (a) : (b))
#define Clamp(a, b, v) Minimum(b, Maximum(a, v))

#define SetFlag(expr, flag)		((expr) |= (flag))
#define ClearFlag(expr, flag)	((expr) &= ~(flag))
#define ToggleFlag(expr, flag)  ((expr) ^= (flag))
#define IsPower2(value) (((value) != 0) && ((value) & ((value) - 1)) == 0)
#define AlignPower2Up(x, p) (((x) + (p) - 1) & ~((p) - 1))
#define AlignPower2Down(x, p) ((x) & ~((p) - 1))

#define ByteSwap16(a) ((((a) & 0x00FF) << 8) | (((a) & 0xFF00) >> 8))
#define ByteSwap32(a) ((((a) & 0x000000FF) << 24) | (((a) & 0x0000FF00) << 8) | \
						(((a) & 0x00FF0000) >> 8) | (((a) & 0xFF000000) >> 24))
#define ByteSwap64(a) ((((a) & 0x00000000000000FFULL) << 56) | (((a) & 0x000000000000FF00ULL) << 40) | \
						(((a) & 0x0000000000FF0000ULL) << 24) | (((a) & 0x00000000FF000000ULL) << 8) | \
						(((a) & 0x000000FF00000000ULL) >> 8) | (((a) & 0x0000FF0000000000ULL) >> 24) | \
						(((a) & 0x00FF000000000000ULL) >> 40) | (((a) & 0xFF00000000000000ULL) >> 56))

#define KiloBytes(n) ((n)*1024u)
#define MegaBytes(n) (KiloBytes(n) * 1024u)
#define GigaBytes(n) (MegaBytes(n) * 1024u)
#define MEMORY_ALLOCATOR_COMMIT_SIZE MegaBytes(64)

#if defined(__cplusplus)
extern "C" {
#endif

	uint8 *AlignPointer(uint8 *location, ptrsize alignment);
	ptrsize   AlignSize(ptrsize location, ptrsize alignment);

	typedef struct memory_arena {
		ptrsize CurrentPos;
		ptrsize CommitPos;
		ptrsize Reserved;
		uint8 *Memory;
	} memory_arena;

	memory_arena MemoryArenaCreate(ptrsize max_size);
	void MemoryArenaDestroy(memory_arena *arena);
	void MemoryArenaReset(memory_arena *arena);

	void *PushSize(memory_arena *arena, ptrsize size);
	void *PushSizeAligned(memory_arena *arena, ptrsize size, uint32 alignment);
	void SetAllocationPosition(memory_arena *arena, ptrsize pos);

#define PushType(arena, type) (type *)PushSize(arena, sizeof(type))
#define PushArray(arena, type, count) (type *)PushSize(arena, sizeof(type) * count)
#define PushArrayAligned(arena, type, count, alignment) (type *)PushSizeAligned(arena, sizeof(type) * count, alignment)

	typedef struct temporary_memory {
		memory_arena *Arena;
		ptrsize Position;
	} temporary_memory;

	temporary_memory BeginTemporaryMemory(memory_arena *arena);
	void  EndTemporaryMemory(temporary_memory *temp);
	void FreeTemporaryMemory(temporary_memory *temp);

	typedef void *(*memory_allocate)(ptrsize size, void *context);
	typedef void *(*memory_reallocate)(void *ptr, ptrsize previous_size, ptrsize new_size, void *context);
	typedef void (*memory_free)(void *ptr, void *context);

	typedef struct memory_allocator {
		memory_allocate Allocate;
		memory_reallocate Reallocate;
		memory_free Free;
		void *Context;
	} memory_allocator;

	memory_allocator MemoryArenaAllocator(memory_arena *arena);

	typedef enum log_kind {
		Log_Kind_Info,
		Log_Kind_Warn,
		Log_Kind_Error
	} log_kind;

	typedef void (*log_procedure)(void *agent, log_kind kind, const char *fmt, va_list list);
	typedef void (*fatal_error_procedure)(const char *message);

	typedef struct log_agent {
		log_procedure	Procedure;
		void *Data;
	} log_agent;

	typedef struct scratchpad {
		memory_arena Arena[2];
	} scratchpad;

	typedef struct thread_context {
		memory_allocator      Allocator;
		scratchpad			  Scratchpad;
		log_agent			  LogAgent;
		fatal_error_procedure FatalError;
	} thread_context;

	extern thread_local thread_context ThreadContext;

	//
	//
	//

	memory_arena *ThreadScratchpad();
	void ResetThreadScratchpad();
	memory_allocator ThreadScratchpadAllocator();

	void FatalError(const char *msg);

	void LogInfoV(const char *fmt, va_list list);
	void LogWarnV(const char *fmt, va_list list);
	void LogErrorV(const char *fmt, va_list list);
	void LogInfo(const char *fmt, ...);
	void LogWarn(const char *fmt, ...);
	void LogError(const char *fmt, ...);

	void InitThreadContext(memory_allocator allocator, uint32_t scratchpad_size, log_agent logger, fatal_error_procedure fatal_error);

	//
	//
	//

	const char *GetPlatformName();

	//
	//
	//

	void *VirtualMemoryAllocate(void *ptr, ptrsize size);
	void *VirtualMemoryCommit(void *ptr, ptrsize size);
	bool VirtualMemoryDecommit(void *ptr, ptrsize size);
	bool VirtualMemoryFree(void *ptr);

#if defined(__cplusplus)
}
#endif

typedef struct String {
	int64_t  Length;
	uint8_t *Data;

#if defined(__cplusplus)
	String() : Data(0), Length(0) {}
	template <int64_t _Length>
	constexpr String(const char(&a)[_Length]) : Data((uint8_t *)a), Length(_Length - 1) {}
	String(const uint8_t *_Data, int64_t _Length) : Data((uint8_t *)_Data), Length(_Length) {}
	const uint8_t &operator[](const int64_t index) const {
		Assert(index < Length);
		return Data[index];
	}
	uint8_t &operator[](const int64_t index) {
		Assert(index < Length);
		return Data[index];
	}
#endif
} String;
#define StringLiteral(lit) (String) { lit, (int64_t)(sizeof(lit) - 1) }


#if defined(__cplusplus)

template <typename T>
struct z_exit_scope {
	T lambda;
	z_exit_scope(T lambda) : lambda(lambda) {}
	~z_exit_scope() { lambda(); }
};
struct z_exit_scope_help {
	template <typename T>
	z_exit_scope<T> operator+(T t) {
		return t;
	}
};
#define Defer const auto &_zConcat(defer__, __LINE__) = z_exit_scope_help() + [&]()


struct scoped_scratchpad {
	temporary_memory Scratchpad;

	scoped_scratchpad() {
		memory_arena *arena = ThreadScratchpad();
		Scratchpad = BeginTemporaryMemory(arena);
	}
	~scoped_scratchpad() {
		EndTemporaryMemory(&Scratchpad);
	}

	memory_arena *operator&() {
		return Scratchpad.Arena;
	}
};


inline void *MemoryAllocate(ptrsize size, memory_allocator &allocator = ThreadContext.Allocator) {
	allocator.Allocate(size, allocator.Context);
}

inline void *MemoryReallocate(ptrsize old_size, ptrsize new_size, void *ptr, memory_allocator &allocator = ThreadContext.Allocator) {
	allocator.Reallocate(ptr, old_size, new_size, allocator.Context);
}

inline void MemoryFree(void *ptr, memory_allocator &allocator = ThreadContext.Allocator) {
	allocator.Free(ptr, allocator.Context);
}

#endif
