#pragma once
#include "KrCommon.h"

struct Semaphore;

Semaphore *Semaphore_Create(int value);
void       Semaphore_Destory(Semaphore *sem);
int        Semaphore_Wait(Semaphore *sem, int millisecs);
bool       Semaphore_Signal(Semaphore *sem);

//
//
//

struct Thread;

typedef int(*Thread_Proc)(void *arg);

Thread *Thread_Create(Thread_Proc proc, void *arg = nullptr, uint32_t scratchpad_size = 0, const Thread_Context_Params &params = ThreadContextDefaultParams);
int     Thread_Wait(Thread *thread, int millisecs);
void    Thread_Terminate(Thread *thread, int code);
void    Thread_Yield();
void    Thread_Sleep(int millisecs);
void    Thread_Exit(int code);
void    Thread_Destroy(Thread *thread);
