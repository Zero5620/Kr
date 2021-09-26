#pragma once
#include "zBase.h"

#if defined(__cplusplus)
extern "C" {
#endif

	log_agent GetCrtLogAgent();
	fatal_error_procedure GetCrtFatalErrorProcedure();
	memory_allocator GetCrtMemoryAllocator();
	void InitThreadContextCrt(uint32_t scratchpad_size);

#if defined(__cplusplus)
}
#endif
