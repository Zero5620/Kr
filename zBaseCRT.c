#include "zBaseCRT.h"

#if defined(__cplusplus)
extern "C" {
#endif
#include <stdlib.h>
#include <stdio.h>

	static void *CRT_Allocate(ptrsize size, void *context) {
		return malloc(size);
	}

	static void *CRT_Reallocate(void *ptr, ptrsize old_size, ptrsize new_size, void *context) {
		return realloc(ptr, new_size);
	}

	static void CRT_Free(void *ptr, void *context) {
		free(ptr);
	}

	static void CRT_Log(void *agent, log_kind kind, const char *fmt, va_list list) {
		FILE *fp = ((kind == Log_Kind_Info)? stdout : stderr);
		vfprintf(fp, fmt, list);
	}

	static void CRT_fatal_error(const char *message) {
		fprintf(stderr, "%s", message);
		exit(EXIT_FAILURE);
	}

	log_agent GetCrtLogAgent() {
		log_agent logger;
		logger.Data = 0;
		logger.Procedure = CRT_Log;
		return logger;
	}

	fatal_error_procedure GetCrtFatalErrorProcedure() {
		return CRT_fatal_error;
	}

	memory_allocator GetCrtMemoryAllocator() {
		memory_allocator allocator;
		allocator.Allocate = CRT_Allocate;
		allocator.Reallocate = CRT_Reallocate;
		allocator.Free = CRT_Free;
		allocator.Context = 0;
		return allocator;
	}

	void InitThreadContextCrt(uint32_t scratchpad_size) {
		memory_allocator allocator = GetCrtMemoryAllocator();
		log_agent logger = GetCrtLogAgent();
		fatal_error_procedure fatal_error = GetCrtFatalErrorProcedure();
		InitThreadContext(allocator, scratchpad_size, logger, fatal_error);
	}

#if defined(__cplusplus)
}
#endif

