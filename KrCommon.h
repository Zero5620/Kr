#pragma once

#include <string.h>

#include "KrPlatform.h"

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
