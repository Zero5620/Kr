#include "zBase.h"
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

	thread_local Thread_Context ThreadContext;

	Uint8 *AlignPointer(Uint8 *location, Ptrsize alignment) {
		return (uint8_t *)((Ptrsize)(location + (alignment - 1)) & ~(alignment - 1));
	}

	Ptrsize AlignSize(Ptrsize location, Ptrsize alignment) {
		return ((location + (alignment - 1)) & ~(alignment - 1));
	}

	Memory_Arena MemoryArenaCreate(Ptrsize max_size) {
		Memory_Arena arena;
		arena.Reserved = max_size;
		arena.Memory = VirtualMemoryAllocate(0, arena.Reserved);
		arena.CommitPos = 0;
		arena.CurrentPos = 0;
		return arena;
	}

	void MemoryArenaDestroy(Memory_Arena *arena) {
		VirtualMemoryFree(arena->Memory);
	}

	void MemoryArenaReset(Memory_Arena *arena) {
		arena->CurrentPos = 0;
	}

	void *PushSize(Memory_Arena *arena, Ptrsize size) {
		void *ptr = 0;
		if (arena->CurrentPos + size <= arena->Reserved) {
			ptr = arena->Memory + arena->CurrentPos;
			arena->CurrentPos += size;
			if (arena->CurrentPos > arena->CommitPos) {
				Ptrsize CommitPos = AlignPower2Up(arena->CurrentPos, MEMORY_ALLOCATOR_COMMIT_SIZE - 1);
				CommitPos = Minimum(CommitPos, arena->Reserved);
				VirtualMemoryCommit(arena->Memory + arena->CommitPos, CommitPos - arena->CommitPos);
				arena->CommitPos = CommitPos;
			}
		}
		return ptr;
	}

	void *PushSizeAligned(Memory_Arena *arena, Ptrsize size, Uint32 alignment) {
		Ptrsize alloc_size = AlignSize(arena->CurrentPos, alignment) - arena->CurrentPos + size;
		return PushSize(arena, size);
	}

	void SetAllocationPosition(Memory_Arena *arena, Ptrsize pos) {
		if (pos < arena->CurrentPos) {
			arena->CurrentPos = pos;
			Ptrsize CommitPos = AlignPower2Up(pos, MEMORY_ALLOCATOR_COMMIT_SIZE - 1);
			CommitPos = Minimum(CommitPos, arena->Reserved);

			if (CommitPos < arena->CommitPos) {
				VirtualMemoryDecommit(arena->Memory + CommitPos, arena->CommitPos - CommitPos);
				arena->CommitPos = CommitPos;
			}
		}
	}

	Temporary_Memory BeginTemporaryMemory(Memory_Arena *arena) {
		Temporary_Memory mem;
		mem.Arena = arena;
		mem.Position = arena->CurrentPos;
		return mem;
	}

	void  EndTemporaryMemory(Temporary_Memory *temp) {
		temp->Arena->CurrentPos = temp->Position;
	}

	void FreeTemporaryMemory(Temporary_Memory *temp) {
		SetAllocationPosition(temp->Arena, temp->Position);
	}

	static void *MemoryArenaAllocatorAllocate(Ptrsize size, void *context) {
		Memory_Arena *arena = (Memory_Arena *)context;
		return PushSizeAligned(arena, size, sizeof(Ptrsize));
	}

	static void *MemoryArenaAllocatorReallocate(void *ptr, Ptrsize previous_size, Ptrsize new_size, void *context) {
		Memory_Arena *arena = (Memory_Arena *)context;

		if (previous_size > new_size) return ptr;

		if (arena->Memory + arena->CurrentPos == ((uint8_t *)ptr + previous_size)) {
			PushSize(arena, new_size - previous_size);
			return ptr;
		}

		void *new_ptr = PushSizeAligned(arena, new_size, sizeof(Ptrsize));
		memmove(ptr, new_ptr, previous_size);
		return new_ptr;
	}

	static void MemoryArenaAllocatorFree(void *ptr, void *context) {}

	Memory_Allocator MemoryArenaAllocator(Memory_Arena *arena) {
		Memory_Allocator allocator;
		allocator.Allocate = MemoryArenaAllocatorAllocate;
		allocator.Reallocate = MemoryArenaAllocatorReallocate;
		allocator.Free = MemoryArenaAllocatorFree;
		allocator.Context = arena;
		return allocator;
	}

	Memory_Arena *ThreadScratchpad() {
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

	Memory_Allocator ThreadScratchpadAllocator() {
		Memory_Arena *arena = ThreadScratchpad();
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

	void InitThreadContext(Memory_Allocator allocator, uint32_t scratchpad_size, Log_Agent logger, Fatal_Error_Procedure fatal_error) {
		ThreadContext.Allocator = allocator;

		if (scratchpad_size) {
			for (Uint32 index = 0; index < ArrayCount(ThreadContext.Scratchpad.Arena); ++index) {
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

	void *VirtualMemoryAllocate(void *ptr, Ptrsize size) {
		return VirtualAlloc(ptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	}

	void *VirtualMemoryCommit(void *ptr, Ptrsize size) {
		return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
	}

	bool VirtualMemoryDecommit(void *ptr, Ptrsize size) {
		return VirtualFree(ptr, size, MEM_DECOMMIT);
	}

	bool VirtualMemoryFree(void *ptr) {
		return VirtualFree(ptr, 0, MEM_RELEASE);
	}

#endif

#if defined(__cplusplus)
}
#endif
