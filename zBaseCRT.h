#pragma once
#include "zBase.h"

#if defined(__cplusplus)
extern "C" {
#endif

	Log_Agent GetCrtLogAgent();
	Fatal_Error_Procedure GetCrtFatalErrorProcedure();
	Memory_Allocator GetCrtMemoryAllocator();
	void InitThreadContextCrt(uint32_t scratchpad_size);

#if defined(__cplusplus)
}
#endif
