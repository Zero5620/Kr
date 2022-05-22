#pragma once
#include "KrCommon.h"

#include <string.h>

template <typename T>
struct Array {
	ptrdiff_t          count;
	T *                data;

	ptrdiff_t          allocated;
	Memory_Allocator   allocator;

	inline Array() : count(0), data(nullptr), allocated(0), allocator(ThreadContext.allocator) {}
	inline Array(Memory_Allocator _allocator) : count(0), data(0), allocated(0), allocator(_allocator) {}
	inline operator Array_View<T>() { return Array_View<T>(data, count); }
	inline operator const Array_View<T>() const { return Array_View<T>(data, count); }
	inline T &operator[](ptrdiff_t i) { Assert(i >= 0 && i < count); return data[i]; }
	inline const T &operator[](ptrdiff_t i) const { Assert(i >= 0 && i < count); return data[i]; }
	inline T *begin() { return data; }
	inline T *end() { return data + count; }
	inline const T *begin() const { return data; }
	inline const T *end() const { return data + count; }
	T &First() { Assert(count); return data[0]; }
	const T &First() const { Assert(count); return data[0]; }
	T &Last() { Assert(count); return data[count - 1]; }
	const T &Last() const { Assert(count); return data[count - 1]; }

	inline ptrdiff_t GetGrowCapacity(ptrdiff_t size) const {
		ptrdiff_t new_capacity = allocated ? (allocated + allocated / 2) : 8;
		return new_capacity > size ? new_capacity : size;
	}

	inline bool Reserve(ptrdiff_t new_capacity) {
		if (new_capacity <= allocated)
			return true;
		T *new_data = (T *)MemoryReallocate(allocated * sizeof(T), new_capacity * sizeof(T), data, allocator);
		if (new_data) {
			data = new_data;
			allocated = new_capacity;
			return true;
		}
		return false;
	}

	inline bool Resize(ptrdiff_t new_count) {
		if (Reserve(new_count)) {
			for (ptrdiff_t index = count; index < new_count; ++index)
				data[index] = T{};
			count = new_count;
			return true;
		}
		return false;
	}

	template <typename... Args> void Emplace(const Args &...args) {
		if (count == allocated) {
			ptrdiff_t n = GetGrowCapacity(allocated + 1);
			if (!Reserve(n)) return;
		}
		data[count] = T(args...);
		count += 1;
	}

	T *Add() {
		if (count == allocated) {
			ptrdiff_t c = GetGrowCapacity(allocated + 1);
			if (!Reserve(c))
				return nullptr;
		}
		count += 1;
		return data + (count - 1);
	}

	T *AddN(uint32_t n) {
		if (count + n > allocated) {
			ptrdiff_t c = GetGrowCapacity(count + n);
			if (!Reserve(c))
				return nullptr;
		}
		T *ptr = data + count;
		count += n;
		return ptr;
	}

	void Add(const T &d) {
		T *m = Add();
		if (m)
			*m = d;
	}

	bool Copy(Array_View<T> src) {
		if (src.count + count >= allocated) {
			ptrdiff_t c = GetGrowCapacity(src.count + count + 1);
			if (Reserve(c))
				return false;
		}
		memcpy(data + count, src.data, src.count * sizeof(T));
		count += src.count;
		return true;
	}

	void RemoveLast() {
		Assert(count > 0);
		count -= 1;
	}

	void Remove(ptrdiff_t index) {
		Assert(index < count);
		memmove(data + index, data + index + 1, (count - index - 1) * sizeof(T));
		count -= 1;
	}

	void RemoveUnordered(ptrdiff_t index) {
		Assert(index < count);
		data[index] = data[count - 1];
		count -= 1;
	}

	void Insert(ptrdiff_t index, const T &v) {
		Assert(index < count + 1);
		if (!Add()) return;
		for (ptrdiff_t move_index = count - 1; move_index > index; --move_index) {
			data[move_index] = data[move_index - 1];
		}
		data[index] = v;
	}

	void InsertUnordered(ptrdiff_t index, const T &v) {
		Assert(index < count + 1);
		if (!Add()) return;
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
		MemoryFree(a->data, sizeof(T) * a->allocated, a->allocator);
}

template <typename T>
inline ptrdiff_t Find(Array_View<T> arr, const T &v) {
	for (ptrdiff_t index = 0; index < arr.count; ++index) {
		auto elem = arr.data + index;
		if (*elem == v) {
			return index;
		}
	}
	return -1;
}

template <typename T, typename SearchFunc, typename... Args>
inline ptrdiff_t Find(Array_View<T> arr, SearchFunc func, const Args &...args) {
	for (ptrdiff_t index = 0; index < arr.count; ++index) {
		if (func(arr[index], args...)) {
			return index;
		}
	}
	return -1;
}

//
//
//

constexpr int HASHTABLE_BUCKET_SIZE = 4;
constexpr int HASHTABLE_BUCKET_SHIFT = 2;
constexpr int HASHTABLE_BUCKET_MASK = HASHTABLE_BUCKET_SIZE - 1;
constexpr int HASHTABLE_EMPTY = 0;
constexpr int HASHTABLE_TOMBSTONE = 1;
constexpr int HASHTABLE_MAX_RES_HASH = 1;
constexpr int HASHTABLE_INITIAL_SIZE = 4;

static_assert(IsPower2(HASHTABLE_BUCKET_SIZE), "");
static_assert(HASHTABLE_BUCKET_SIZE == 8 || HASHTABLE_BUCKET_SIZE == 4, "");

static uint32_t SDBM(const uint8_t *buff, ptrdiff_t len) {
	uint32_t hash = 0;
	for (ptrdiff_t i = 0; i < len; ++i)
		hash = buff[i] + (hash << 6) + (hash << 16) - hash;
	return hash;
}

static inline ptrdiff_t NextPowerOf2(ptrdiff_t n) {
	if (IsPower2(n))
		return n;
	uint32_t count = 0;
	while (n != 0) {
		n >>= 1;
		count += 1;
	}
	return (ptrdiff_t)1 << (ptrdiff_t)count;
}

template <typename T> struct Hasher_Default {
	size_t operator()(const T v) const {
		return SDBM((uint8_t *)&v, sizeof(v));
	}
};
template <> struct Hasher_Default<String> {
	size_t operator()(const String v) const {
		return SDBM(v.data, v.length);
	}
};
template <> struct Hasher_Default<ptrdiff_t> {
	size_t operator()(const ptrdiff_t v) const {
		return (uint32_t)v;
	}
};
template <>
struct Hasher_Default<uint64_t> {
	size_t operator()(const uint64_t v) const {
		return (uint32_t)v;
	}
};
template <>
struct Hasher_Default<int> {
	size_t operator()(const uint64_t v) const {
		return (uint32_t)v;
	}
};

template <typename T>
struct Trivial_Key_Alloc {
	T operator()(const T &key) const { return key; }
};

template <typename T>
struct Trivial_Key_Free {
	void operator()(const T *key) {}
};

template <typename K, typename V,
	typename Hasher = Hasher_Default<K>,
	typename Key_Alloc = Trivial_Key_Alloc<K>,
	typename Key_Free = Trivial_Key_Free<K>>
struct Hash_Table {
	struct Bucket {
		size_t    hash[HASHTABLE_BUCKET_SIZE];
		ptrdiff_t index[HASHTABLE_BUCKET_SIZE];
	};

	struct Pair {
		K key;
		V value;
	};

	Bucket *         buckets;
	ptrdiff_t        count;
	ptrdiff_t        p2allocated;
	ptrdiff_t        tombstones;
	Array<Pair>      storage;
	Memory_Allocator allocator;

	Hasher           hasher;
	Key_Alloc        key_alloc;
	Key_Free         key_free;

	Hash_Table() : buckets(nullptr), count(0), p2allocated(0), tombstones(0), storage(ThreadContext.allocator), allocator(ThreadContext.allocator) {}
	Hash_Table(Memory_Allocator _allocator) : buckets(nullptr), count(0), p2allocated(0), tombstones(0), storage(_allocator), allocator(_allocator) {}

	inline Pair *begin() { return storage.begin(); }
	inline Pair *end() { return storage.end(); }
	inline const Pair *begin() const { return storage.begin(); }
	inline const Pair *end() const { return storage.end(); }

	size_t GetHash(const K key) const {
		size_t hash = hasher(key);
		if (hash <= HASHTABLE_MAX_RES_HASH)
			hash += HASHTABLE_MAX_RES_HASH + 1;
		return hash;
	}

	bool Resize(ptrdiff_t new_p2allocated) {
		Assert(new_p2allocated >= count);

		new_p2allocated = NextPowerOf2(new_p2allocated);

		Bucket *nbuckets = (Bucket *)MemoryAllocate(sizeof(Bucket) * (new_p2allocated >> HASHTABLE_BUCKET_SHIFT));
		if (!nbuckets) return false;

		memset(nbuckets, 0, sizeof(Bucket) * (new_p2allocated >> HASHTABLE_BUCKET_SHIFT));

		if (buckets) {
			ptrdiff_t src_bucket_count = p2allocated >> HASHTABLE_BUCKET_SHIFT;

			for (ptrdiff_t i = 0; i < src_bucket_count; ++i) {
				Bucket *src_bucket = &buckets[i];
				for (size_t j = 0; j < HASHTABLE_BUCKET_SIZE; ++j) {
					if (src_bucket->hash[j] <= HASHTABLE_MAX_RES_HASH)
						continue;

					size_t    hash = src_bucket->hash[j];
					ptrdiff_t pos = hash & (new_p2allocated - 1);
					ptrdiff_t step = HASHTABLE_BUCKET_SIZE;

					bool inserted = false;
					while (!inserted) {
						Bucket *dst_bucket = &nbuckets[pos >> HASHTABLE_BUCKET_SHIFT];

						for (int iter = (int)(pos & HASHTABLE_BUCKET_MASK); iter < HASHTABLE_BUCKET_SIZE; ++iter) {
							if (dst_bucket->hash[iter] == HASHTABLE_EMPTY) {
								dst_bucket->hash[iter]  = hash;
								dst_bucket->index[iter] = src_bucket->index[j];
								inserted = true;
								break;
							}
						}

						if (!inserted) {
							auto limit = pos & HASHTABLE_BUCKET_MASK;
							for (auto iter = 0; iter < limit; ++iter) {
								if (dst_bucket->hash[iter] == HASHTABLE_EMPTY) {
									dst_bucket->hash[iter] = hash;
									dst_bucket->index[iter] = src_bucket->index[j];
									inserted = true;
									break;
								}
							}
						}

						pos += step;
						pos &= (new_p2allocated - 1);
						step += HASHTABLE_BUCKET_SIZE;
					}
				}
			}

			MemoryFree(buckets, (p2allocated >> HASHTABLE_BUCKET_SHIFT) * sizeof(Bucket), allocator);
		}

		buckets = nbuckets;
		p2allocated = new_p2allocated;
		tombstones = 0;

		return true;
	}

	ptrdiff_t FindSlot(const K key) const {
		if (!count)
			return -1;

		size_t hash = GetHash(key);
		ptrdiff_t step = HASHTABLE_BUCKET_SIZE;
		ptrdiff_t pos = hash & (p2allocated - 1);

		while (1) {
			Bucket *bucket = &buckets[pos >> HASHTABLE_BUCKET_SHIFT];

			for (int iter = (int)(pos & HASHTABLE_BUCKET_MASK); iter < HASHTABLE_BUCKET_SIZE; ++iter) {
				if (bucket->hash[iter] == hash && storage[bucket->index[iter]].key == key)
					return (pos & ~HASHTABLE_BUCKET_MASK) + iter;
				else if (bucket->hash[iter] == HASHTABLE_EMPTY)
					return -1;
			}

			int limit = (int)(pos & HASHTABLE_BUCKET_MASK);
			for (int iter = 0; iter < limit; ++iter) {
				if (bucket->hash[iter] == hash && storage[bucket->index[iter]].key == key)
					return (pos & ~HASHTABLE_BUCKET_MASK) + iter;
				else if (bucket->hash[iter] == HASHTABLE_EMPTY)
					return -1;
			}

			pos += step;
			pos &= (p2allocated - 1);
			step += HASHTABLE_BUCKET_SIZE;
		}

		Unreachable();
		return -1;
	}

	V *Find(const K key) {
		ptrdiff_t slot = FindSlot(key);
		if (slot >= 0) {
			Bucket *bucket = &buckets[slot >> HASHTABLE_BUCKET_SHIFT];
			int index = (int)(slot & HASHTABLE_BUCKET_MASK);
			return &storage[bucket->index[index]].value;
		}
		return nullptr;
	}

	const V *Find(const K key) const {
		ptrdiff_t slot = FindSlot(key);
		if (slot >= 0) {
			Bucket *bucket = &buckets[slot >> HASHTABLE_BUCKET_SHIFT];
			int index = (int)(slot & HASHTABLE_BUCKET_MASK);
			return &storage[bucket->index[index]].value;
		}
		return nullptr;
	}

	Pair *AllocateNode() {
		return storage.Add();
	}

	void FreeNode(ptrdiff_t index) {
		ptrdiff_t last = storage.count - 1;
		if (index != last) {
			storage[index] = storage[last];

			ptrdiff_t pos = FindSlot(storage[index].key);
			Assert(pos >= 0);

			Bucket *bucket = &buckets[pos >> HASHTABLE_BUCKET_SHIFT];
			int iter       = (int)(pos & HASHTABLE_BUCKET_MASK);
			Assert(bucket->index[iter] == last);
			bucket->index[iter] = index;
		}
		storage.count -= 1;
	}

	V *FindOrDefault(const K key, const V &def) {
		ptrdiff_t used_count_threshold = p2allocated - (p2allocated >> 2);
		if (count >= used_count_threshold) {
			ptrdiff_t new_p2allocated = p2allocated ? p2allocated * 2 : HASHTABLE_INITIAL_SIZE;
			Resize(new_p2allocated);
		}

		size_t hash = GetHash(key);
		ptrdiff_t step = HASHTABLE_BUCKET_SIZE;
		ptrdiff_t pos = hash & (p2allocated - 1);

		ptrdiff_t tombstone_pos = -1;

		while (1) {
			Bucket *bucket = &buckets[pos >> HASHTABLE_BUCKET_SHIFT];

			for (int iter = (int)(pos & HASHTABLE_BUCKET_MASK); iter < HASHTABLE_BUCKET_SIZE; ++iter) {
				if (bucket->hash[iter] == hash && storage[bucket->index[iter]].key == key) {
					return &storage[bucket->index[iter]].value;
				} else if (bucket->hash[iter] == HASHTABLE_EMPTY) {
					pos = (pos & ~HASHTABLE_BUCKET_MASK) + iter;
					goto EmptyFound;
				} else if (tombstone_pos < 0 && bucket->hash[iter] == HASHTABLE_TOMBSTONE) {
					tombstone_pos = (pos & ~HASHTABLE_BUCKET_MASK) + iter;
				}
			}

			int limit = (int)(pos & HASHTABLE_BUCKET_MASK);
			for (int iter = 0; iter < limit; ++iter) {
				if (bucket->hash[iter] == hash && storage[bucket->index[iter]].key == key) {
					return &storage[bucket->index[iter]].value;
				} else if (bucket->hash[iter] == HASHTABLE_EMPTY) {
					pos = (pos & ~HASHTABLE_BUCKET_MASK) + iter;
					goto EmptyFound;
				} else if (tombstone_pos < 0 && bucket->hash[iter] == HASHTABLE_TOMBSTONE) {
					tombstone_pos = (pos & ~HASHTABLE_BUCKET_MASK) + iter;
				}
			}

			pos += step;
			pos &= (p2allocated - 1);
			step += HASHTABLE_BUCKET_SIZE;
		}

	EmptyFound:
		if (count == p2allocated)
			return nullptr;

		Pair *pair = AllocateNode();
		if (pair) {
			if (tombstone_pos >= 0) {
				pos = tombstone_pos;
				tombstones -= 1;
			}

			count += 1;

			Bucket *bucket = &buckets[pos >> HASHTABLE_BUCKET_SHIFT];

			int index = (int)(pos & HASHTABLE_BUCKET_MASK);
			bucket->hash[index]  = hash;
			bucket->index[index] = storage.count - 1;

			pair->key   = key_alloc(key);
			pair->value = def;

			return &pair->value;
		}

		return nullptr;
	}

	void Put(const K key, const V &value) {
		auto dst = FindOrDefault(key, V{});
		if (dst) {
			*dst = value;
		}
	}

	void Remove(const K key) {
		if (!count) return;

		ptrdiff_t pos = FindSlot(key);
		if (pos < 0) return;

		Bucket *bucket = &buckets[pos >> HASHTABLE_BUCKET_SHIFT];
		int index = (int)(pos & HASHTABLE_BUCKET_MASK);

		ptrdiff_t to_free    = bucket->index[index];
		bucket->hash[index]  = HASHTABLE_TOMBSTONE;
		bucket->index[index] = -1;

		key_free(&storage[to_free].key);

		FreeNode(to_free);

		count -= 1;
		tombstones += 1;

		ptrdiff_t shrink_threshold = p2allocated >> 2;
		ptrdiff_t tombstone_threshold = (p2allocated >> 3) + (p2allocated >> 4);
		if (count < shrink_threshold && p2allocated > HASHTABLE_INITIAL_SIZE)
			Resize(p2allocated >> 2);
		else if (tombstones > tombstone_threshold)
			Resize(p2allocated);
	}
};

template <typename K, typename V, typename Hasher, typename Key_Alloc, typename Key_Free>
void Free(Hash_Table<K, V, Hasher, Key_Alloc, Key_Free> *table) {
	ptrdiff_t bucket_count = table->p2allocated >> HASHTABLE_BUCKET_SHIFT;
	MemoryFree(table->buckets, sizeof(typename Hash_Table<K, V, Hasher, Key_Alloc, Key_Free>::Bucket) * bucket_count, table->allocator);
	for (auto &pair : table->storage) {
		table->key_free(&pair.key);
	}
	Free(&table->storage);
	*table = Hash_Table<K, V, Hasher, Key_Alloc, Key_Free>(table->allocator);
}
