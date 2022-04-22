// Compilation
// MSVC: cl .\Tests.cpp /I../ -FeTests /std:c++17

#define ASSERTION_HANDLED

#include "KrCommon.h"
#include "KrCommon.cpp"

#include <stdio.h>

void AssertHandle(const char *reason, const char *file, int line, const char *proc) {
	printf("[ ERROR ] --- Assertion Failed: %s:%d, procedure: %s\n", file, line, proc);
}

void Test_MemoryArena() {
	size_t cap = MegaBytes(64);
	Memory_Arena *arena = MemoryArenaAllocate(cap, MemoryArenaCommitSize);
	Assert(arena);

	Assert(MemoryArenaCapSize(arena) == cap);
	Assert(MemoryArenaUsedSize(arena) == sizeof(Memory_Arena));
	Assert(MemoryArenaEmptySize(arena) == cap - sizeof(Memory_Arena));
	Assert(arena->committed == MemoryArenaCommitSize);

	if (arena) {
		int arr_count = 0;
		int *values = PushArray(arena, int, arr_count);
		Assert(values);

		int used = sizeof(Memory_Arena) + arr_count * sizeof(int);
		int left = cap - sizeof(Memory_Arena) - arr_count * sizeof(int);

		Assert(MemoryArenaUsedSize(arena) == used);
		Assert(MemoryArenaEmptySize(arena) == left);

		memset(values, 0xcd, arr_count * sizeof(int));

		{
			auto temp = BeginTemporaryMemory(arena);

			int size = KiloBytes(64);
			void *mem = PushSize(arena, size);
			memset(mem, 0xcd, size);

			used += size;
			left -= size;

			Assert(MemoryArenaUsedSize(arena) == used);
			Assert(MemoryArenaEmptySize(arena) == left);
			Assert(arena->committed == 2 * MemoryArenaCommitSize);

			MemoryArenaResize(arena, 0);
			Assert(arena->committed == MemoryArenaCommitSize);
		}
	}
}

int main(int argc, char **argv) {
	Test_MemoryArena();

	printf("Finished\n");
	return 0;
}
