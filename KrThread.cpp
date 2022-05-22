#include "KrThread.h"

#if PLATFORM_WINDOWS == 1
#include <Windows.h>

struct Semaphore { ptrdiff_t __unused; };

Semaphore *Semaphore_Create(int value) {
	HANDLE handle = CreateSemaphoreW(nullptr, value, INT32_MAX, nullptr);
	return (Semaphore *)handle;
}

void Semaphore_Destory(Semaphore *sem) {
	CloseHandle((HANDLE)sem);
}

int Semaphore_Wait(Semaphore *sem, int millisecs) {
	DWORD res = WaitForSingleObject((HANDLE)sem, millisecs >= 0 ? millisecs : INFINITE);
	if (res == WAIT_OBJECT_0) return 1;
	if (res == WAIT_TIMEOUT) return 0;
	return -1;
}

bool Semaphore_Signal(Semaphore *sem) {
	return ReleaseSemaphore((HANDLE)sem, 1, 0);
}

//
//
//

struct Thread {
	HANDLE                handle;
	Thread_Proc           proc;
	void *                arg;
	uint32_t              scratchpad_size;
	Thread_Context_Params params;
};

static DWORD Thread_WindowsThreadProc(LPVOID arg) {
	Thread *thrd = (Thread *)arg;
	InitThreadContext(thrd->scratchpad_size, thrd->params);
	int result = thrd->proc(thrd->arg);
	return result;
}

Thread *Thread_Create(Thread_Proc proc, void *arg, uint32_t scratchpad_size, const Thread_Context_Params &params) {
	Thread *thrd = new Thread;
	if (thrd) {
		thrd->proc            = proc;
		thrd->arg             = arg;
		thrd->scratchpad_size = scratchpad_size;
		thrd->params          = params;
		thrd->handle          = CreateThread(nullptr, 0, Thread_WindowsThreadProc, thrd, 0, 0);
	}
	return thrd;
}

int Thread_Wait(Thread *thread, int millisecs) {
	DWORD res = WaitForSingleObject(thread->handle, millisecs >= 0 ? millisecs : INFINITE);
	if (res == WAIT_OBJECT_0) return 1;
	if (res == WAIT_TIMEOUT) return 0;
	return -1;
}

void Thread_Terminate(Thread *thread, int code) {
	TerminateThread(thread->handle, code);
}

void Thread_Yield() {
	SwitchToThread();
}

void Thread_Sleep(int millisecs) {
	Sleep(millisecs);
}

void Thread_Exit(int code) {
	TerminateThread(GetCurrentThread(), code);
}

void Thread_Destroy(Thread *thread) {
	CloseHandle(thread->handle);
	MemoryFree(thread, sizeof(*thread));
}

#endif
