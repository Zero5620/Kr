#pragma once
#include <stdint.h>
#include "KrCommon.h"

#if PLATFORM_WINDOWS == 1 && (ARCH_X64 == 1 || ARCH_X86 == 1)
#include <intrin.h>

inproc int32_t AtomicInc(int32_t volatile *addend) { return _InterlockedIncrement((volatile long *)addend); }
inproc int32_t AtomicDec(int32_t volatile *addend) { return _InterlockedDecrement((volatile long *)addend); }
inproc int32_t AtomicAdd(int32_t volatile *addend, int32_t value) { return _interlockedadd((volatile long *)addend, value); }
inproc int32_t AtomicSub(int32_t volatile *sub, int32_t value) { return _interlockedadd((volatile long *)sub, -value); }
inproc int32_t AtomicCmpExg(int32_t volatile *dst, int32_t exchange, int32_t comperand) { return _InterlockedCompareExchange((volatile long *)dst, exchange, comperand); }
inproc void *  AtomicCmpExg(void *volatile *dst, void *exchange, void *comperand) { return _InterlockedCompareExchangePointer(dst, exchange, comperand); }
inproc int32_t AtomicLoad(int32_t volatile *src) { return *src; }
inproc int32_t AtomicExchange(int32_t volatile *src, int32_t value) { return _InterlockedExchange((volatile long *)src, value); }
inproc void    AtomicStore(int32_t volatile *src, int32_t value) { _InterlockedExchange((volatile long *)src, value); }
inproc void *  AtomicLoad(void *volatile *dst) { return *dst; }
inproc void *  AtomicExchange(void *volatile *src, void *value) { return _InterlockedExchangePointer(src, value); }
inproc void    AtomicStore(void *volatile *src, void *value) { _InterlockedExchangePointer(src, value); }

#if ARCH_X64 == 1
inproc int64_t AtomicInc(int64_t volatile *addend) { return _InterlockedIncrement64((volatile long long *)addend); }
inproc int64_t AtomicDec(int64_t volatile *addend) { return _InterlockedDecrement64((volatile long long *)addend); }
inproc int64_t AtomicAdd(int64_t volatile *addend, int64_t value) { return _interlockedadd64((volatile long long *)addend, value); }
inproc int64_t AtomicSub(int64_t volatile *sub, int64_t value) { return _interlockedadd64((volatile long long *)sub, -value); }
inproc int64_t AtomicCmpExg(int64_t volatile *dst, int64_t exchange, int64_t comperand) { return _InterlockedCompareExchange64((volatile long long *)dst, exchange, comperand); }
inproc int64_t AtomicLoad(int64_t volatile *src) { return *src; }
inproc int64_t AtomicExchange(int64_t volatile *src, int64_t value) { return _InterlockedExchange64((volatile long long *)src, value); }
inproc void    AtomicStore(int64_t volatile *src, int64_t value) { _InterlockedExchange64((volatile long long *)src, value); }
#endif

#else

inproc int32_t AtomicInc(int32_t volatile *addend) { return __sync_add_and_fetch(addend, 1); }
inproc int32_t AtomicDec(int32_t volatile *addend) { return __sync_sub_and_fetch(addend, 1); }
inproc int32_t AtomicAdd(int32_t volatile *addend, int32_t value) { return __sync_add_and_fetch(addend, value); }
inproc int32_t AtomicSub(int32_t volatile *sub, int32_t value) { return __sync_sub_and_fetch(sub, value); }
inproc int32_t AtomicCmpExg(int32_t volatile *dst, int32_t exchange, int32_t comperand) { return __sync_val_compare_and_swap(dst, comperand, exchange); }
inproc void *  AtomicCmpExg(void *volatile *dst, void *exchange, void *comperand) { return __sync_val_compare_and_swap(dst, comperand, exchange); }
inproc int32_t AtomicLoad(int32_t volatile *src) { return __atomic_load_n(src, __ATOMIC_SEQ_CST); }
inproc int32_t AtomicExchange(int32_t volatile *src, int32_t value) { return __atomic_exchange_n(src, value, __ATOMIC_SEQ_CST); }
inproc void    AtomicStore(int32_t volatile *src, int32_t value) { __atomic_store_n(src, value, __ATOMIC_SEQ_CST); }
inproc void *  AtomicLoad(void *volatile *dst) { return __atomic_load_n(src, __ATOMIC_SEQ_CST); }
inproc void *  AtomicExchange(void *volatile *src, void *value) { return __atomic_exchange_n(src, value, __ATOMIC_SEQ_CST); }
inproc void    AtomicStore(void *volatile *src, void *value) { __atomic_store_n(src, value, __ATOMIC_SEQ_CST); }

#if ARCH_X64 == 1 || ARCH_ARM64 == 1
inproc int64_t AtomicInc(int64_t volatile *addend) { return __sync_add_and_fetch(addend, 1); }
inproc int64_t AtomicDec(int64_t volatile *addend) { return __sync_add_and_fetch(addend, 1); }
inproc int64_t AtomicAdd(int64_t volatile *addend, int64_t value) { return __sync_add_and_fetch(addend, value); }
inproc int64_t AtomicSub(int64_t volatile *sub, int64_t value) { return __sync_sub_and_fetch(sub, value); }
inproc int64_t AtomicCmpExg(int64_t volatile *dst, int64_t exchange, int64_t comperand) { return __sync_val_compare_and_swap(dst, comperand, exchange); }
inproc int64_t AtomicLoad(int64_t volatile *src) { return __atomic_load_n(src, __ATOMIC_SEQ_CST); }
inproc int64_t AtomicExchange(int64_t volatile *src, int64_t value) { return __atomic_exchange_n(src, value, __ATOMIC_SEQ_CST); }
inproc void    AtomicStore(int64_t volatile *src, int64_t value) { __atomic_store_n(src, value, __ATOMIC_SEQ_CST); }
#endif

#endif

template <typename T>
inproc void *AtomicCmpExg(T *volatile *dst, T *exchange, T *comperand) {
	return (T *)AtomicCmpExg((void *volatile *)dst, (void *)exchange, (void *)comperand);
}

//
//
//

struct Atomic_Guard {
	int32_t volatile value;
};

inproc void SpinLock(Atomic_Guard *guard) {
	while (AtomicCmpExg(&guard->value, 1, 0) == 1)
		;
}

inproc void SpinUnlock(Atomic_Guard *guard) {
	AtomicStore(&guard->value, 0);
}
