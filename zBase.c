#include "zBase.h"
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

	thread_local thread_context ThreadContext;

	uint8 *AlignPointer(uint8 *location, ptrsize alignment) {
		return (uint8_t *)((ptrsize)(location + (alignment - 1)) & ~(alignment - 1));
	}

	ptrsize AlignSize(ptrsize location, ptrsize alignment) {
		return ((location + (alignment - 1)) & ~(alignment - 1));
	}

	memory_arena MemoryArenaCreate(ptrsize max_size) {
		memory_arena arena;
		arena.Reserved = max_size;
		arena.Memory = VirtualMemoryAllocate(0, arena.Reserved);
		arena.CommitPos = 0;
		arena.CurrentPos = 0;
		return arena;
	}

	void MemoryArenaDestroy(memory_arena *arena) {
		VirtualMemoryFree(arena->Memory);
	}

	void MemoryArenaReset(memory_arena *arena) {
		arena->CurrentPos = 0;
	}

	void *PushSize(memory_arena *arena, ptrsize size) {
		void *ptr = 0;
		if (arena->CurrentPos + size <= arena->Reserved) {
			ptr = arena->Memory + arena->CurrentPos;
			arena->CurrentPos += size;
			if (arena->CurrentPos > arena->CommitPos) {
				ptrsize CommitPos = AlignPower2Up(arena->CurrentPos, MEMORY_ALLOCATOR_COMMIT_SIZE - 1);
				CommitPos = Minimum(CommitPos, arena->Reserved);
				VirtualMemoryCommit(arena->Memory + arena->CommitPos, CommitPos - arena->CommitPos);
				arena->CommitPos = CommitPos;
			}
		}
		return ptr;
	}

	void *PushSizeAligned(memory_arena *arena, ptrsize size, uint32 alignment) {
		ptrsize alloc_size = AlignSize(arena->CurrentPos, alignment) - arena->CurrentPos + size;
		return PushSize(arena, size);
	}

	void SetAllocationPosition(memory_arena *arena, ptrsize pos) {
		if (pos < arena->CurrentPos) {
			arena->CurrentPos = pos;
			ptrsize CommitPos = AlignPower2Up(pos, MEMORY_ALLOCATOR_COMMIT_SIZE - 1);
			CommitPos = Minimum(CommitPos, arena->Reserved);

			if (CommitPos < arena->CommitPos) {
				VirtualMemoryDecommit(arena->Memory + CommitPos, arena->CommitPos - CommitPos);
				arena->CommitPos = CommitPos;
			}
		}
	}

	temporary_memory BeginTemporaryMemory(memory_arena *arena) {
		temporary_memory mem;
		mem.Arena = arena;
		mem.Position = arena->CurrentPos;
		return mem;
	}

	void  EndTemporaryMemory(temporary_memory *temp) {
		temp->Arena->CurrentPos = temp->Position;
	}

	void FreeTemporaryMemory(temporary_memory *temp) {
		SetAllocationPosition(temp->Arena, temp->Position);
	}

	static void *MemoryArenaAllocatorAllocate(ptrsize size, void *context) {
		memory_arena *arena = (memory_arena *)context;
		return PushSizeAligned(arena, size, sizeof(ptrsize));
	}

	static void *MemoryArenaAllocatorReallocate(void *ptr, ptrsize previous_size, ptrsize new_size, void *context) {
		memory_arena *arena = (memory_arena *)context;

		if (previous_size > new_size) return ptr;

		if (arena->Memory + arena->CurrentPos == ((uint8_t *)ptr + previous_size)) {
			PushSize(arena, new_size - previous_size);
			return ptr;
		}

		void *new_ptr = PushSizeAligned(arena, new_size, sizeof(ptrsize));
		memmove(ptr, new_ptr, previous_size);
		return new_ptr;
	}

	static void MemoryArenaAllocatorFree(void *ptr, void *context) {}

	memory_allocator MemoryArenaAllocator(memory_arena *arena) {
		memory_allocator allocator;
		allocator.Allocate = MemoryArenaAllocatorAllocate;
		allocator.Reallocate = MemoryArenaAllocatorReallocate;
		allocator.Free = MemoryArenaAllocatorFree;
		allocator.Context = arena;
		return allocator;
	}

	memory_arena *ThreadScratchpad() {
		for (uint32_t index = 0; ArrayCount(ThreadContext.Scratchpad.Arena); ++index) {
			if (&ThreadContext.Scratchpad.Arena[index] != ThreadContext.Allocator.Context) {
				return &ThreadContext.Scratchpad.Arena[index];
			}
		}
		return 0;
	}

	void ResetThreadScratchpad() {
		for (uint32_t index = 0; index < ArrayCount(ThreadContext.Scratchpad.Arena); ++index) {
			MemoryArenaReset(&ThreadContext.Scratchpad.Arena[index]);
		}
	}

	memory_allocator ThreadScratchpadAllocator() {
		memory_arena *arena = ThreadScratchpad();
		return MemoryArenaAllocator(arena);
	}

	void FatalError(const char *msg) {
		ThreadContext.FatalError(msg);
	}

	void LogInfoV(const char *fmt, va_list list) {
		ThreadContext.LogAgent.Procedure(ThreadContext.LogAgent.Data, Log_Kind_Info, fmt, list);
	}

	void LogWarnV(const char *fmt, va_list list) {
		ThreadContext.LogAgent.Procedure(ThreadContext.LogAgent.Data, Log_Kind_Warn, fmt, list);
	}

	void LogErrorV(const char *fmt, va_list list) {
		ThreadContext.LogAgent.Procedure(ThreadContext.LogAgent.Data, Log_Kind_Error, fmt, list);
	}

	void LogInfo(const char *fmt, ...) {
		va_list args;
		va_start(args, fmt);
		ThreadContext.LogAgent.Procedure(ThreadContext.LogAgent.Data, Log_Kind_Info, fmt, args);
		va_end(args);
	}

	void LogWarn(const char *fmt, ...) {
		va_list args;
		va_start(args, fmt);
		ThreadContext.LogAgent.Procedure(ThreadContext.LogAgent.Data, Log_Kind_Warn, fmt, args);
		va_end(args);
	}

	void LogError(const char *fmt, ...) {
		va_list args;
		va_start(args, fmt);
		ThreadContext.LogAgent.Procedure(ThreadContext.LogAgent.Data, Log_Kind_Error, fmt, args);
		va_end(args);
	}

	void InitThreadContext(memory_allocator allocator, uint32_t scratchpad_size, log_agent logger, fatal_error_procedure fatal_error) {
		ThreadContext.Allocator = allocator;

		if (scratchpad_size) {
			for (uint32 index = 0; index < ArrayCount(ThreadContext.Scratchpad.Arena); ++index) {
				ThreadContext.Scratchpad.Arena[index] = MemoryArenaCreate(scratchpad_size);
			}
		} else {
			memset(&ThreadContext.Scratchpad, 0, sizeof(ThreadContext.Scratchpad));
		}

		ThreadContext.LogAgent = logger;
		ThreadContext.FatalError = fatal_error;
	}

	const char *GetPlatformName() {
#if OS_ANDRIOD == 1
		return "Andriod";
#endif
#if OS_LINUX == 1
		return "Linux";
#endif
#if OS_MAC == 1
		return "Mac";
#endif
#if OS_IOS == 1
		return "iOS";
#endif
#if OS_WINDOWS == 1
		return "Windows";
#endif
	}

#if OS_WINDOWS == 1
#define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS 0
#include <Windows.h>

	void *VirtualMemoryAllocate(void *ptr, ptrsize size) {
		return VirtualAlloc(ptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	}

	void *VirtualMemoryCommit(void *ptr, ptrsize size) {
		return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
	}

	bool VirtualMemoryDecommit(void *ptr, ptrsize size) {
		return VirtualFree(ptr, size, MEM_DECOMMIT);
	}

	bool VirtualMemoryFree(void *ptr) {
		return VirtualFree(ptr, 0, MEM_RELEASE);
	}

#endif

#if defined(__cplusplus)
}
#endif
