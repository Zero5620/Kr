#pragma once
#include "KrCommon.h"

#include <string.h>

#define ForEach(c) for (auto it = IterBegin(&(c)); IterEnd(&(c), it); IterNext(&it)) 
#define ForEachTag(name, c) for (auto name = IterBegin(&(c)); IterEnd(&(c), name); IterNext(&name)) 

template <typename T> 
struct Array_View {
	int64_t count;
	T *data;

	inline Array_View() : count(0), data(nullptr) {}
	inline Array_View(T *p, int64_t n) : count(n), data(p) {}
	template <int64_t _Count> constexpr Array_View(const T(&a)[_Count]) : count(_Count), data((T *)a) {}
	inline T &operator[](int64_t index) const { Assert(index < count); return data[index]; }
	inline T *begin() { return data; }
	inline T *end() { return data + count; }
	inline const T *begin() const { return data; }
	inline const T *end() const { return data + count; }
	T &First() { Assert(count); return data[0]; }
	const T &First() const { Assert(count); return data[0]; }
	T &Last() { Assert(count); return data[count - 1]; }
	const T &Last() const { Assert(count); return data[count - 1]; }
};

template <typename T>
struct Array {
	int64_t          count;
	T *data;

	int64_t          allocated;
	Memory_Allocator allocator;

	inline Array() : count(0), data(nullptr), allocated(0), allocator(ThreadContext.allocator) {}
	inline Array(Memory_Allocator _allocator) : count(0), data(0), allocated(0), allocator(_allocator) {}
	inline operator Array_View<T>() { return Array_View<T>(data, count); }
	inline operator const Array_View<T>() const { return Array_View<T>(data, count); }
	inline T &operator[](int64_t i) { Assert(i >= 0 && i < count); return data[i]; }
	inline const T &operator[](int64_t i) const { Assert(i >= 0 && i < count); return data[i]; }
	inline T *begin() { return data; }
	inline T *end() { return data + count; }
	inline const T *begin() const { return data; }
	inline const T *end() const { return data + count; }
	T &First() { Assert(count); return data[0]; }
	const T &First() const { Assert(count); return data[0]; }
	T &Last() { Assert(count); return data[count - 1]; }
	const T &Last() const { Assert(count); return data[count - 1]; }

	inline int64_t GetGrowCapacity(int64_t size) const {
		int64_t new_capacity = allocated ? (allocated + allocated / 2) : 8;
		return new_capacity > size ? new_capacity : size;
	}

	inline void Reserve(int64_t new_capacity) {
		if (new_capacity <= allocated)
			return;
		T *new_data = (T *)MemoryReallocate(allocated * sizeof(T), new_capacity * sizeof(T), data, allocator);
		if (new_data) {
			data = new_data;
			allocated = new_capacity;
		}
	}

	inline void Resize(int64_t new_count) {
		Reserve(new_count);
		count = new_count;
	}

	template <typename... Args> void Emplace(const Args &...args) {
		if (count == allocated) {
			int64_t n = GetGrowCapacity(allocated + 1);
			Reserve(n);
		}
		data[count] = T(args...);
		count += 1;
	}

	T *Add() {
		if (count == allocated) {
			int64_t c = GetGrowCapacity(allocated + 1);
			Reserve(c);
		}
		count += 1;
		return data + (count - 1);
	}

	T *AddN(uint32_t n) {
		if (count + n > allocated) {
			int64_t c = GetGrowCapacity(count + n);
			Reserve(c);
		}
		T *ptr = data + count;
		count += n;
		return ptr;
	}

	void Add(const T &d) {
		T *m = Add();
		*m = d;
	}

	void Copy(Array_View<T> src) {
		if (src.count + count >= allocated) {
			int64_t c = GetGrowCapacity(src.count + count + 1);
			Reserve(c);
		}
		memcpy(data + count, src.data, src.count * sizeof(T));
		count += src.count;
	}

	void RemoveBack() {
		Assert(count > 0);
		count -= 1;
	}

	void Remove(int64_t index) {
		Assert(index < count);
		memmove(data + index, data + index + 1, (count - index - 1) * sizeof(T));
		count -= 1;
	}

	void RemoveUnordered(int64_t index) {
		Assert(index < count);
		data[index] = data[count - 1];
		count -= 1;
	}

	void Insert(int64_t index, const T &v) {
		Assert(index < count + 1);
		Add();
		for (int64_t move_index = count - 1; move_index > index; --move_index) {
			data[move_index] = data[move_index - 1];
		}
		data[index] = v;
	}

	void InsertUnordered(int64_t index, const T &v) {
		Assert(index < count + 1);
		Add();
		data[count - 1] = data[index];
		data[index] = v;
	}

	void Pack() {
		if (count != allocated) {
			data = (T *)MemoryReallocate(allocated * sizeof(T), count * sizeof(T), data, allocator);
			allocated = count;
		}
	}

	void Reset() {
		count = 0;
	}
};

template <typename T>
inline void Free(Array<T> *a) {
	if (a->data)
		MemoryFree(a->data, a->allocator);
}

template <typename T>
inline void Free(Array_View<T> *a) {
	if (a->data)
		MemoryFree(a->data);
}

template <typename T>
inline int64_t Find(Array_View<T> arr, const T &v) {
	for (int64_t index = 0; index < arr.count; ++index) {
		auto elem = arr.data + index;
		if (*elem == v) {
			return index;
		}
	}
	return -1;
}

template <typename T, typename SearchFunc, typename... Args>
inline int64_t Find(Array_View<T> arr, SearchFunc func, const Args &...args) {
	for (int64_t index = 0; index < arr.count; ++index) {
		if (func(arr[index], args...)) {
			return index;
		}
	}
	return -1;
}

//
//
//
