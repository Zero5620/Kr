#include "KrCommon.h"

#include <string.h>
#include <stdio.h>

thread_local Thread_Context ThreadContext;

void HandleAssertion(const char *file, int line, const char *proc) {
	ThreadContext.assert_proc(file, line, proc);
	TriggerBreakpoint();
}

struct Memory_Arena {
	size_t current;
	size_t reserved;
	size_t committed;
};

bool operator==(const String a, const String b) {
	if (a.length != b.length)
		return false;
	return memcmp(a.data, b.data, a.length) == 0;
}

bool operator!=(const String a, const String b) {
	if (a.length != b.length)
		return true;
	return memcmp(a.data, b.data, a.length) != 0;
}

uint8_t *AlignPointer(uint8_t *location, size_t alignment) {
	return (uint8_t *)((size_t)(location + (alignment - 1)) & ~(alignment - 1));
}

Memory_Arena *MemoryArenaAllocate(size_t max_size, size_t initial_size) {
	max_size = AlignPower2Up(max_size, 64 * 1024);
	uint8_t *mem = (uint8_t *)VirtualMemoryAllocate(0, max_size);
	if (mem) {
		size_t commit_size = AlignPower2Up(initial_size, MemoryArenaCommitSize);
		commit_size = Clamp(MemoryArenaCommitSize, max_size, commit_size);
		if (VirtualMemoryCommit(mem, commit_size)) {
			Memory_Arena *arena = (Memory_Arena *)mem;
			arena->current = sizeof(Memory_Arena);
			arena->reserved = max_size;
			arena->committed = commit_size;
			return arena;
		}
		VirtualMemoryFree(mem, max_size);
	}
	return nullptr;
}

void MemoryArenaFree(Memory_Arena *arena) {
	VirtualMemoryFree(arena, arena->reserved);
}

void MemoryArenaReset(Memory_Arena *arena) {
	arena->current = sizeof(Memory_Arena);
}

size_t MemoryArenaCapSize(Memory_Arena *arena) {
	return arena->reserved;
}

size_t MemoryArenaUsedSize(Memory_Arena *arena) {
	return arena->current;
}

size_t MemoryArenaEmptySize(Memory_Arena *arena) {
	return arena->reserved - arena->current;
}

bool MemoryArenaEnsureCommit(Memory_Arena *arena, size_t pos) {
	if (pos <= arena->committed) {
		return true;
	}

	pos = Maximum(pos, MemoryArenaCommitSize);
	uint8_t *mem = (uint8_t *)arena;

	size_t committed = AlignPower2Up(pos, MemoryArenaCommitSize);
	committed = Minimum(committed, arena->reserved);
	if (VirtualMemoryCommit(mem + arena->committed, committed - arena->committed)) {
		arena->committed = committed;
		return true;
	}
	return false;
}

bool MemoryArenaSetPos(Memory_Arena *arena, size_t pos) {
	if (MemoryArenaEnsureCommit(arena, pos)) {
		arena->current = pos;
		return true;
	}
	return false;
}

bool MemoryArenaPackToPos(Memory_Arena *arena, size_t pos) {
	if (MemoryArenaSetPos(arena, pos)) {
		size_t committed = AlignPower2Up(pos, MemoryArenaCommitSize);
		committed = Clamp(MemoryArenaCommitSize, arena->reserved, committed);

		uint8_t *mem = (uint8_t *)arena;
		if (committed < arena->committed) {
			if (VirtualMemoryDecommit(mem + committed, arena->committed - committed))
				arena->committed = committed;
		}
		return true;
	}
	return false;
}

bool MemoryArenaAlignCurrent(Memory_Arena *arena, size_t alignment) {
	uint8_t *mem     = (uint8_t *)arena + arena->current;
	uint8_t *aligned = AlignPointer(mem, alignment);
	size_t pos       = arena->current + (aligned - mem);
	if (MemoryArenaSetPos(arena, pos))
		return true;
	return false;
}

void *MemoryArenaGetCurrent(Memory_Arena *arena) {
	uint8_t *mem = (uint8_t *)arena;
	return mem + arena->current;
}

void *PushSize(Memory_Arena *arena, size_t size) {
	uint8_t *mem = (uint8_t *)arena + arena->current;
	size_t pos   = arena->current + size;
	if (MemoryArenaSetPos(arena, pos))
		return mem;
	return 0;
}

void *PushSizeAligned(Memory_Arena *arena, size_t size, uint32_t alignment) {
	if (MemoryArenaAlignCurrent(arena, alignment))
		return PushSize(arena, size);
	return nullptr;
}

Temporary_Memory BeginTemporaryMemory(Memory_Arena *arena) {
	Temporary_Memory mem;
	mem.arena = arena;
	mem.position = arena->current;
	return mem;
}

void *PushSizeZero(Memory_Arena *arena, size_t size) {
	void *result = PushSize(arena, size);
	MemoryZeroSize(result, size);
	return result;
}

void *PushSizeAlignedZero(Memory_Arena *arena, size_t size, uint32_t alignment) {
	void *result = PushSizeAligned(arena, size, alignment);
	MemoryZeroSize(result, size);
	return result;
}

void PopSize(Memory_Arena *arena, size_t size) {
	size_t pos = arena->current - size;
	Assert(pos >= sizeof(Memory_Arena) && pos <= arena->reserved);
	MemoryArenaSetPos(arena, pos);
}

void EndTemporaryMemory(Temporary_Memory *temp) {
	temp->arena->current = temp->position;
}

void FreeTemporaryMemory(Temporary_Memory *temp) {
	MemoryArenaSetPos(temp->arena, temp->position);
	MemoryArenaPackToPos(temp->arena, temp->position);
}

Memory_Arena *ThreadScratchpad(uint32_t index) {
	return ThreadContext.scratchpad.arena[index];
}

void ResetThreadScratchpad() {
	for (auto thread_arena : ThreadContext.scratchpad.arena) {
		MemoryArenaReset(thread_arena);
	}
}

static void *MemoryArenaAllocatorAllocate(size_t size, void *context) {
	Memory_Arena *arena = (Memory_Arena *)context;
	return PushSizeAligned(arena, size, sizeof(size_t));
}

static void *MemoryArenaAllocatorReallocate(void *ptr, size_t previous_size, size_t new_size, void *context) {
	Memory_Arena *arena = (Memory_Arena *)context;
	uint8_t *mem        = (uint8_t *)arena;

	if (previous_size >= new_size) {
		if (mem + arena->current == ((uint8_t *)ptr + previous_size))
			PopSize(arena, previous_size - new_size);
		return ptr;
	}

	if (mem + arena->current == ((uint8_t *)ptr + previous_size)) {
		if (PushSize(arena, new_size - previous_size))
			return ptr;
		return 0;
	}

	void *new_ptr = PushSizeAligned(arena, new_size, sizeof(size_t));
	if (new_ptr) {
		memmove(new_ptr, ptr, previous_size);
		return new_ptr;
	}
	return 0;
}

static void MemoryArenaAllocatorFree(void *ptr, size_t allocated, void *context) {
	Memory_Arena *arena = (Memory_Arena *)context;

	auto current = (uint8_t *)arena + arena->current;
	auto end_ptr = (uint8_t *)ptr + allocated;

	if (current == end_ptr) {
		arena->current -= allocated;
	}
}

static void *MemoryArenaAllocatorProc(Allocation_Kind kind, void *mem, size_t prev_size, size_t new_size, void *context) {
	if (kind == ALLOCATION_KIND_ALLOC) {
		return MemoryArenaAllocatorAllocate(new_size, context);
	} else if (kind == ALLOCATION_KIND_REALLOC) {
		return MemoryArenaAllocatorReallocate(mem, prev_size, new_size, context);
	} else {
		MemoryArenaAllocatorFree(mem, prev_size, context);
		return nullptr;
	}
}

Memory_Allocator MemoryArenaAllocator(Memory_Arena *arena) {
	Memory_Allocator allocator;
	allocator.proc = MemoryArenaAllocatorProc;
	allocator.context = arena;
	return allocator;
}

static void  InitOSContent();
static void  FatalErrorOS(const char *message);
static void *DefaultMemoryAllocateProc(size_t size, void *context);
static void *DefaultMemoryReallocateProc(void *ptr, size_t previous_size, size_t new_size, void *context);
static void  DefaultMemoryFreeProc(void *ptr, size_t allocated, void *context);

void *Thread_Context::DefaultMemoryAllocatorProc(Allocation_Kind kind, void *mem, size_t prev_size, size_t new_size, void *context) {
	if (kind == ALLOCATION_KIND_ALLOC) {
		return DefaultMemoryAllocateProc(new_size, context);
	} else if (kind == ALLOCATION_KIND_REALLOC) {
		return DefaultMemoryReallocateProc(mem, prev_size, new_size, context);
	} else {
		DefaultMemoryFreeProc(mem, prev_size, context);
		return nullptr;
	}
}

void *Thread_Context::TmpMemoryAllocatorProc(Allocation_Kind kind, void *mem, size_t prev_size, size_t new_size, void *context) {
	for (int i = MaxThreadContextScratchpadArena - 1; i >= 0; --i) {
		if (ThreadContext.allocator.context != ThreadContext.scratchpad.arena[i]) {
			return MemoryArenaAllocatorProc(kind, mem, prev_size, new_size, ThreadContext.scratchpad.arena[i]);
		}
	}
	Unreachable();
	return nullptr;
}

void Thread_Context::DefaultLoggerProc(void *context, Log_Level level, const char *source, const char *fmt, va_list args) {
	char buff[4096 + 2];
	int len = vsnprintf(buff, 4096, fmt, args);
	snprintf(buff + len, 2, "\n");
	fprintf(level != LOG_LEVEL_ERROR ? stdout : stderr, "%s", buff);
}

void Thread_Context::DefaultFatalError(const char *message) {
	LogErrorEx("Fatal Error", message);
	FatalErrorOS(message);
}

void InitThreadContext(uint32_t scratchpad_size, const Thread_Context_Params &params) {
	InitOSContent();

	if (scratchpad_size) {
		for (uint32_t arena_index = 0; arena_index < MaxThreadContextScratchpadArena; ++arena_index) {
			ThreadContext.scratchpad.arena[arena_index] = MemoryArenaAllocate(scratchpad_size);
		}
	} else {
		memset(&ThreadContext.scratchpad, 0, sizeof(ThreadContext.scratchpad));
	}

	ThreadContext.allocator   = params.allocator;
	ThreadContext.logger      = params.logger;
	ThreadContext.assert_proc = params.assert_proc;
	ThreadContext.fatal_error = params.fatal_error;
}

//
//
//

void *MemoryAllocate(size_t size, Memory_Allocator allocator) {
	return allocator.proc(ALLOCATION_KIND_ALLOC, nullptr, 0, size, allocator.context);
}

void *MemoryReallocate(size_t old_size, size_t new_size, void *ptr, Memory_Allocator allocator) {
	return allocator.proc(ALLOCATION_KIND_REALLOC, ptr, old_size, new_size, allocator.context);
}

void MemoryFree(void *ptr, size_t allocated, Memory_Allocator allocator) {
	allocator.proc(ALLOCATION_KIND_FREE, ptr, allocated, 0, allocator.context);
}

void *operator new(size_t size, Memory_Allocator allocator) {
	return allocator.proc(ALLOCATION_KIND_ALLOC, nullptr, 0, size, allocator.context);
}

void *operator new[](size_t size, Memory_Allocator allocator) {
	return allocator.proc(ALLOCATION_KIND_ALLOC, nullptr, 0, size, allocator.context);
}

void *operator new(size_t size) {
	return ThreadContext.allocator.proc(ALLOCATION_KIND_ALLOC, nullptr, 0, size, ThreadContext.allocator.context);
}

void *operator new[](size_t size) {
	return ThreadContext.allocator.proc(ALLOCATION_KIND_ALLOC, nullptr, 0, size, ThreadContext.allocator.context);
}

void operator delete(void *ptr, Memory_Allocator allocator) {
	allocator.proc(ALLOCATION_KIND_FREE, ptr, 0, 0, allocator.context);
}

void operator delete[](void *ptr, Memory_Allocator allocator) {
	allocator.proc(ALLOCATION_KIND_FREE, ptr, 0, 0, allocator.context);
}

void operator delete(void *ptr) noexcept {
	ThreadContext.allocator.proc(ALLOCATION_KIND_FREE, ptr, 0, 0, ThreadContext.allocator.context);
}

void operator delete[](void *ptr) noexcept {
	ThreadContext.allocator.proc(ALLOCATION_KIND_FREE, ptr, 0, 0, ThreadContext.allocator.context);
}

//
//
//

void LogExV(Log_Level level, const char *source, const char *fmt, va_list args) {
	ThreadContext.logger.proc(ThreadContext.logger.context, level, source, fmt, args);
}

void LogEx(Log_Level level, const char *source, const char *fmt, ...) {	
	va_list args;
	va_start(args, fmt);
	LogExV(level, source, fmt, args);
	va_end(args);
}

void FatalError(const char *message) {
	ThreadContext.fatal_error(message);
}

void Thread_Context::DefaultHandleAssertion(const char *file, int line, const char *proc) {
	LogError("Assertion Failed inside \"%s\": %s(%d)", proc, file, line);
}

//
//
//

#if PLATFORM_WINDOWS == 1
#define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS 0
#include <Windows.h>

static void InitOSContent() {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
}

static void FatalErrorOS(const char *message) {
	wchar_t wmessage[4096];
	int wlen = MultiByteToWideChar(CP_UTF8, 0, message, (int)strlen(message), wmessage, 4095);
	wmessage[wlen] = 0;
	FatalAppExitW(0, wmessage);
}

static void *DefaultMemoryAllocateProc(size_t size, void *context) {
	HANDLE heap = GetProcessHeap();
	return HeapAlloc(heap, 0, size);
}

static void *DefaultMemoryReallocateProc(void *ptr, size_t previous_size, size_t new_size, void *context) {
	HANDLE heap = GetProcessHeap();
	if (ptr) {
		return HeapReAlloc(heap, 0, ptr, new_size);
	} else {
		return HeapAlloc(heap, 0, new_size);
	}
}

static void DefaultMemoryFreeProc(void *ptr, size_t allocated, void *context) {
	HANDLE heap = GetProcessHeap();
	HeapFree(heap, 0, ptr);
}

void *VirtualMemoryAllocate(void *ptr, size_t size) {
	return VirtualAlloc(ptr, size, MEM_RESERVE, PAGE_READWRITE);
}

bool VirtualMemoryCommit(void *ptr, size_t size) {
	return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) != NULL;
}

bool VirtualMemoryDecommit(void *ptr, size_t size) {
	return VirtualFree(ptr, size, MEM_DECOMMIT);
}

bool VirtualMemoryFree(void *ptr, size_t size) {
	return VirtualFree(ptr, 0, MEM_RELEASE);
}

#endif

#if PLATFORM_LINUX == 1 || PLATFORM_MAC == 1
#include <sys/mman.h>
#include <stdlib.h>

static void InitOSContent() {}

static void FatalErrorOS(const char *message) {
	exit(1);
}

static void *DefaultMemoryAllocateProc(size_t size, void *context) {
	return malloc(size);
}

static void *DefaultMemoryReallocateProc(void *ptr, size_t previous_size, size_t new_size, void *context) {
	return realloc(ptr, new_size);
}

static void DefaultMemoryFreeProc(void *ptr, size_t allocated, void *context) {
	free(ptr);
}

void *VirtualMemoryAllocate(void *ptr, size_t size) {
	void *result = mmap(ptr, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (result == MAP_FAILED)
		return NULL;
	return result;
}

bool VirtualMemoryCommit(void *ptr, size_t size) {
	return mprotect(ptr, size, PROT_READ | PROT_WRITE) == 0;
}

bool VirtualMemoryDecommit(void *ptr, size_t size) {
	return mprotect(ptr, size, PROT_NONE) == 0;
}

bool VirtualMemoryFree(void *ptr, size_t size) {
	return munmap(ptr, size) == 0;
}

#endif
