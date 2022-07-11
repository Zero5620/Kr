#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "KrPlatform.h"

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
#define INLINE_PROCEDURE static inline
#else
#define INLINE_PROCEDURE inline
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

#if !defined(ASSERTION_HANDLED)
#define AssertHandle(reason, file, line, proc) TriggerBreakpoint()
#else
void AssertHandle(const char *reason, const char *file, int line, const char *proc);
#endif

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
#define DebugTriggerbreakpoint TriggerBreakpoint
#define Assert(x)                                                             \
    do                                                                        \
    {                                                                         \
        if (!(x))                                                             \
            AssertHandle("Assert Failed", __FILE__, __LINE__, __PROCEDURE__); \
    } while (0)
#else
#define DebugTriggerbreakpoint()
#define Assert(x) \
    do            \
    {             \
        0;        \
    } while (0)
#endif

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
#define Unimplemented() AssertHandle("Unimplemented procedure", __FILE__, __LINE__, __PROCEDURE__);
#else
#define Unimplemented() TriggerBreakpoint();
#endif

#ifdef __GNUC__
[[noreturn]] inline __attribute__((always_inline)) void Unreachable() { DebugTriggerbreakpoint(); __builtin_unreachable(); }
#elif defined(_MSC_VER)
[[noreturn]] __forceinline void Unreachable() { DebugTriggerbreakpoint(); __assume(false); }
#else // ???
inline void Unreachable() { TriggerBreakpoint(); }
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

typedef uint32_t boolx;

constexpr size_t MemoryArenaCommitSize = KiloBytes(64);

struct String {
	ptrdiff_t length;
	uint8_t * data;

	String() : data(0), length(0) {}
	template <ptrdiff_t _Length>
	constexpr String(const char(&a)[_Length]) : data((uint8_t *)a), length(_Length - 1) {}
	String(const uint8_t *_Data, ptrdiff_t _Length) : data((uint8_t *)_Data), length(_Length) {}
	String(const char *_Data, ptrdiff_t _Length) : data((uint8_t *)_Data), length(_Length) {}
	const uint8_t &operator[](const ptrdiff_t index) const { Assert(index < length); return data[index]; }
	uint8_t &operator[](const ptrdiff_t index) { Assert(index < length); return data[index]; }
	inline uint8_t *begin() { return data; }
	inline uint8_t *end() { return data + length; }
	inline const uint8_t *begin() const { return data; }
	inline const uint8_t *end() const { return data + length; }
};

typedef String Buffer;

#define StrFmt    "%.*s"
#define StrArg(s) (int)(s).length, (s).data

bool operator==(const String a, const String b);
bool operator!=(const String a, const String b);

template <typename T>
struct Array_View {
	ptrdiff_t count;
	T *data;

	inline Array_View() : count(0), data(nullptr) {}
	inline Array_View(T *p, ptrdiff_t n) : count(n), data(p) {}
	template <ptrdiff_t _Count> constexpr Array_View(const T(&a)[_Count]) : count(_Count), data((T *)a) {}
	inline T &operator[](int64_t index) const { Assert(index < count); return data[index]; }
	inline T *begin() { return data; }
	inline T *end() { return data + count; }
	inline const T *begin() const { return data; }
	inline const T *end() const { return data + count; }
};

#define _zConcatInternal(x, y) x##y
#define _zConcat(x, y) _zConcatInternal(x, y)

template <typename T>
struct Exit_Scope {
	T lambda;
	Exit_Scope(T lambda) : lambda(lambda) {
	}
	~Exit_Scope() {
		lambda();
	}
};
struct Exit_Scope_Help {
	template <typename T>
	Exit_Scope<T> operator+(T t) {
		return t;
	}
};
#define Defer const auto &_zConcat(defer__, __LINE__) = Exit_Scope_Help() + [&]()

//
//
//

uint8_t *AlignPointer(uint8_t *location, size_t alignment);

struct Memory_Arena;

Memory_Arena *MemoryArenaAllocate(size_t max_size, size_t commit_size = MemoryArenaCommitSize);
void MemoryArenaFree(Memory_Arena *arena);
void MemoryArenaReset(Memory_Arena *arena);
size_t MemoryArenaCapSize(Memory_Arena *arena);
size_t MemoryArenaUsedSize(Memory_Arena *arena);
size_t MemoryArenaEmptySize(Memory_Arena *arena);

bool  MemoryArenaEnsureCommit(Memory_Arena *arena, size_t pos);
bool  MemoryArenaSetPos(Memory_Arena *arena, size_t pos);
bool  MemoryArenaPackToPos(Memory_Arena *arena, size_t pos);
bool  MemoryArenaAlignCurrent(Memory_Arena *arena, size_t alignment);
void *MemoryArenaGetCurrent(Memory_Arena *arena);

#define MemoryZeroSize(mem, size) memset(mem, 0, size)
#define MemoryZero(var) MemoryZeroSize(&var, sizeof(var))

void *PushSize(Memory_Arena *arena, size_t size);
void *PushSizeAligned(Memory_Arena *arena, size_t size, uint32_t alignment);
void *PushSizeZero(Memory_Arena *arena, size_t size);
void *PushSizeAlignedZero(Memory_Arena *arena, size_t size, uint32_t alignment);

#define PushType(arena, type) (type *)PushSizeAligned(arena, sizeof(type), alignof(type))
#define PushTypeZero(arena, type) (type *)PushSizeAlignedZero(arena, sizeof(type), alignof(type))
#define PushArray(arena, type, count) (type *)PushSizeAligned(arena, sizeof(type) * (count), alignof(type))
#define PushArrayZero(arena, type, count) (type *)PushSizeAlignedZero(arena, sizeof(type) * (count), alignof(type))
#define PushArrayAligned(arena, type, count, alignment) (type *)PushSizeAligned(arena, sizeof(type) * (count), alignment)
#define PushArrayAlignedZero(arena, type, count, alignment) (type *)PushSizeAlignedZero(arena, sizeof(type) * (count), alignment)

void PopSize(Memory_Arena *arena, size_t size);

typedef struct Temporary_Memory {
	Memory_Arena *arena;
	size_t position;
} Temporary_Memory;

Temporary_Memory BeginTemporaryMemory(Memory_Arena *arena);
void EndTemporaryMemory(Temporary_Memory *temp);
void FreeTemporaryMemory(Temporary_Memory *temp);

#ifndef THREAD_CONTEXT_SCRATCHPAD_MAX_ARENAS
#define THREAD_CONTEXT_SCRATCHPAD_MAX_ARENAS 1
#endif // !THREAD_CONTEXT_SCRATCHPAD_MAX_ARENAS

constexpr uint32_t MaxThreadContextScratchpadArena = THREAD_CONTEXT_SCRATCHPAD_MAX_ARENAS;

struct Thread_Scratchpad {
	Memory_Arena *arena[MaxThreadContextScratchpadArena];
};

//
//
//

enum Allocation_Kind { ALLOCATION_KIND_ALLOC, ALLOCATION_KIND_REALLOC, ALLOCATION_KIND_FREE };
typedef void *(*Memory_Allocator_Proc)(Allocation_Kind kind, void *mem, size_t prev_size, size_t new_size, void *context);

struct Memory_Allocator {
	Memory_Allocator_Proc proc;
	void *                context;
};

//
//
//

enum Log_Level { LOG_LEVEL_INFO, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR };
typedef void(*Log_Proc)(void *context, Log_Level level, const char *source, const char *fmt, va_list args);

struct Logger {
	Log_Proc proc;
	void *   context;
};

typedef void(*Fatal_Error_Proc)(const char *message);

//
//
//

typedef struct Thread_Context {
	Thread_Scratchpad scratchpad;
	Memory_Allocator  allocator;
	Logger            logger;
	Fatal_Error_Proc  fatal_error;
} Thread_Context;

extern thread_local Thread_Context ThreadContext;

//
//
//

Memory_Arena *ThreadScratchpad();
Memory_Arena *ThreadScratchpadI(uint32_t i);
Memory_Arena *ThreadUnusedScratchpad(Memory_Arena **arenas, uint32_t count);
void ResetThreadScratchpad();

void ThreadContextSetAllocator(Memory_Allocator allocator);
void ThreadContextSetLogger(Logger logger);

Memory_Allocator MemoryArenaAllocator(Memory_Arena *arena);
Memory_Allocator NullMemoryAllocator();

//
//
//

struct Thread_Context_Params {
	Memory_Allocator allocator;
	Logger           logger;
	Fatal_Error_Proc fatal_error;
	uint32_t         scratchpad_arena_count;
};

void *DefaultMemoryAllocate(size_t size, void *context);
void *DefaultMemoryReallocate(void *ptr, size_t previous_size, size_t new_size, void *context);
void DefaultMemoryFree(void *ptr, size_t allocated, void *context);

void *DefaultMemoryAllocatorProc(Allocation_Kind kind, void *mem, size_t prev_size, size_t new_size, void *context);
void DefaultLoggerProc(void *context, Log_Level level, const char *source, const char *fmt, va_list args);
void DefaultFatalErrorProc(const char *message);

static constexpr Thread_Context_Params ThreadContextDefaultParams = {
	{ DefaultMemoryAllocatorProc, nullptr },
	{ DefaultLoggerProc, nullptr },
	DefaultFatalErrorProc,
	MaxThreadContextScratchpadArena
};

void InitThreadContext(uint32_t scratchpad_size, const Thread_Context_Params &params = ThreadContextDefaultParams);


void *MemoryAllocate(size_t size, Memory_Allocator allocator = ThreadContext.allocator);
void *MemoryReallocate(size_t old_size, size_t new_size, void *ptr, Memory_Allocator allocator = ThreadContext.allocator);
void MemoryFree(void *ptr, size_t allocated, Memory_Allocator allocator = ThreadContext.allocator);

void *operator new(size_t size, Memory_Allocator allocator);
void *operator new[](size_t size, Memory_Allocator allocator);
void *operator new(size_t size);
void *operator new[](size_t size);
void  operator delete(void *ptr, Memory_Allocator allocator);
void  operator delete[](void *ptr, Memory_Allocator allocator);
void  operator delete(void *ptr) noexcept;
void  operator delete[](void *ptr) noexcept;

//
//
//

void LogExV(Log_Level level, const char *source, const char *fmt, va_list args);
#define LogInfoExV(source, fmt, args)    LogExV(LOG_LEVEL_INFO, source, fmt, args)
#define LogWarningExV(source, fmt, args) LogExV(LOG_LEVEL_WARNING, source, fmt, args)
#define LogErrorExV(source, fmt, args)   LogExV(LOG_LEVEL_ERROR, source, fmt, args)

#define LogV(level, fmt, args) LogExV(level, "", fmt, args)
#define LogInfoV(fmt, args)    LogExV(LOG_LEVEL_INFO, "", fmt, args)
#define LogWarningV(fmt, args) LogExV(LOG_LEVEL_WARNING, "", fmt, args)
#define LogErrorV(fmt, args)   LogExV(LOG_LEVEL_ERROR, "", fmt, args)

void LogEx(Log_Level level, const char *source, const char *fmt, ...);
#define LogInfoEx(source, fmt, ...)    LogEx(LOG_LEVEL_INFO, source, fmt, ##__VA_ARGS__)
#define LogWarningEx(source, fmt, ...) LogEx(LOG_LEVEL_WARNING, source, fmt, ##__VA_ARGS__)
#define LogErrorEx(source, fmt, ...)   LogEx(LOG_LEVEL_ERROR, source, fmt, ##__VA_ARGS__)

#define Log(level, fmt, ...) LogEx(level, "", fmt, ##__VA_ARGS__)
#define LogInfo(fmt, ...)    LogEx(LOG_LEVEL_INFO, "", fmt, ##__VA_ARGS__)
#define LogWarning(fmt, ...) LogEx(LOG_LEVEL_WARNING, "", fmt, ##__VA_ARGS__)
#define LogError(fmt, ...)   LogEx(LOG_LEVEL_ERROR, "", fmt, ##__VA_ARGS__)

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
#define Trace(fmt, ...) LogEx(LOG_LEVEL_INFO, "Trace", fmt, ##__VA_ARGS__)
#define TraceEx         LogInfoEx
#else
#define Trace(...) 
#define TraceEx(...) 
#endif

void FatalError(const char *message);

//
//
//

void *VirtualMemoryAllocate(void *ptr, size_t size);
bool VirtualMemoryCommit(void *ptr, size_t size);
bool VirtualMemoryDecommit(void *ptr, size_t size);
bool VirtualMemoryFree(void *ptr, size_t size);
