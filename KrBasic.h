#pragma once
#include "KrBase.h"

#include <string.h>

template <typename T> struct Array_View
{
    Int64 Count;
    T    *Data;

    inline Array_View() : Count(0), Data(nullptr)
    {
    }
    inline Array_View(T *p, Int64 n) : Count(n), Data(p)
    {
    }
    template <Int64 _Count> constexpr Array_View(const T (&a)[_Count]) : Count(_Count), Data((T *)a)
    {
    }
    inline T &operator[](Int64 index) const
    {
        Assert(index < Count);
        return Data[index];
    }
    inline T *begin()
    {
        return Data;
    }
    inline T *end()
    {
        return Data + Count;
    }
    inline const T *begin() const
    {
        return Data;
    }
    inline const T *end() const
    {
        return Data + Count;
    }
};

template <typename T> struct Array
{
    Int64            Count;
    T               *Data;

    Int64            Capacity;
    Memory_Allocator Allocator;

    inline Array() : Count(0), Data(nullptr), Capacity(0), Allocator(ThreadContext.Allocator)
    {
    }
    inline Array(Memory_Allocator allocator) : Count(0), Data(0), Capacity(0), Allocator(allocator)
    {
    }

    inline operator Array_View<T>()
    {
        return Array_View<T>(Data, Count);
    }
    inline operator const Array_View<T>() const
    {
        return Array_View<T>(Data, Count);
    }

    inline T &operator[](Int64 i)
    {
        Assert(i >= 0 && i < Count);
        return Data[i];
    }
    inline const T &operator[](Int64 i) const
    {
        Assert(i >= 0 && i < Count);
        return Data[i];
    }

    inline T *begin()
    {
        return Data;
    }
    inline T *end()
    {
        return Data + Count;
    }
    inline const T *begin() const
    {
        return Data;
    }
    inline const T *end() const
    {
        return Data + Count;
    }

    inline Int64 GetGrowCapacity(Int64 size) const
    {
        Int64 new_capacity = Capacity ? (Capacity + Capacity / 2) : 8;
        return new_capacity > size ? new_capacity : size;
    }

    inline void Reserve(Int64 new_capacity)
    {
        if (new_capacity <= Capacity)
            return;
        T *data = (T *)MemoryReallocate(Capacity * sizeof(T), new_capacity * sizeof(T), Data, &Allocator);
        if (data)
        {
            Data     = data;
            Capacity = new_capacity;
        }
    }

    T *FirstElement()
    {
        Assert(Count);
        return Data;
    }

    T *LastElement()
    {
        Assert(Count);
        return Data + Count - 1;
    }

    template <typename... Args> void Emplace(const Args &...args)
    {
        if (Count == Capacity)
        {
            Int64 n = GetGrowCapacity(Capacity + 1);
            Reserve(n);
        }
        Data[Count] = T(args...);
        Count += 1;
    }

    T *Add()
    {
        if (Count == Capacity)
        {
            Int64 c = GetGrowCapacity(Capacity + 1);
            Reserve(c);
        }
        Count += 1;
        return Data + (Count - 1);
    }

    T *AddN(uint32_t n)
    {
        if (Count + n > Capacity)
        {
            Int64 c = GetGrowCapacity(Count + n);
            Reserve(c);
        }
        T *ptr = Data + Count;
        Count += n;
        return ptr;
    }

    void Add(const T &d)
    {
        T *m = Add();
        *m   = d;
    }

    void Copy(Array_View<T> src)
    {
        if (src.Count + Count >= Capacity)
        {
            Int64 c = GetGrowCapacity(src.Count + Count + 1);
            Reserve(c);
        }
        memcpy(Data + Count, src.Data, src.Count * sizeof(T));
        Count += src.Count;
    }

    void RemoveBack()
    {
        Assert(Count > 0);
        Count -= 1;
    }

    void Remove(Int64 index)
    {
        Assert(index < Count);
        memmove(Data + index, Data + index + 1, (Count - index - 1) * sizeof(T));
        Count -= 1;
    }

    void RemoveUnordered(Int64 index)
    {
        Assert(index < Count);
        Data[index] = Data[Count - 1];
        Count -= 1;
    }

    void Insert(Int64 index, const T &v)
    {
        Assert(index < Count + 1);
        Add();
        for (Int64 move_index = Count - 1; move_index > index; --move_index)
        {
            Data[move_index] = Data[move_index - 1];
        }
        Data[index] = v;
    }

    void InsertUnordered(Int64 index, const T &v)
    {
        Assert(index < Count + 1);
        Add();
        Data[Count - 1] = Data[index];
        Data[index] = v;
    }

    Int64 Find(const T &v) const
    {
        for (Int64 index = 0; index < Count; ++index)
        {
            auto elem = Data + index;
            if (*elem == v)
            {
                return index;
            }
        }
        return -1;
    }

    template <typename SearchFunc, typename... Args> Int64 Find(SearchFunc func, const Args &...args) const
    {
        for (Int64 index = 0; index < Count; ++index)
        {
            if (func(Data[index], args...))
            {
                return index;
            }
        }
        return -1;
    }

    void Reset()
    {
        Count = 0;
    }
};

template <typename T> inline void Free(Array<T> *a)
{
    if (a->Data)
        MemoryFree(a->Data, &a->Allocator);
}
