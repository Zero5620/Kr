#include "KrBase.h"

#if defined(__cplusplus)
extern "C"
{
#endif
#include <stdlib.h>
#include <stdio.h>

    static void *CrtAllocate(Ptrsize size, void *context)
    {
        return malloc(size);
    }

    static void *CrtReallocate(void *ptr, Ptrsize old_size, Ptrsize new_size, void *context)
    {
        return realloc(ptr, new_size);
    }

    static void CrtFree(void *ptr, void *context)
    {
        free(ptr);
    }

    static void CrtLog(void *agent, Log_Kind kind, const char *fmt, va_list list)
    {
        FILE *fp = ((kind == Log_Kind_Info) ? stdout : stderr);
        vfprintf(fp, fmt, list);
    }

    static void CrtFatalError(const char *message)
    {
        fprintf(stderr, "%s", message);
        exit(EXIT_FAILURE);
    }

    Log_Agent GetCrtLogAgent()
    {
        Log_Agent logger;
        logger.Data      = 0;
        logger.Procedure = CrtLog;
        return logger;
    }

    Fatal_Error_Procedure GetCrtFatalErrorProcedure()
    {
        return CrtFatalError;
    }

    Memory_Allocator GetCrtMemoryAllocator()
    {
        Memory_Allocator allocator;
        allocator.Allocate   = CrtAllocate;
        allocator.Reallocate = CrtReallocate;
        allocator.Free       = CrtFree;
        allocator.Context    = 0;
        return allocator;
    }

    void InitThreadContextCrt(uint32_t scratchpad_size)
    {
        Memory_Allocator      allocator   = GetCrtMemoryAllocator();
        Log_Agent             logger      = GetCrtLogAgent();
        Fatal_Error_Procedure fatal_error = GetCrtFatalErrorProcedure();
        InitThreadContext(allocator, scratchpad_size, logger, fatal_error);
    }

#if defined(__cplusplus)
}
#endif
