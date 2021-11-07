#pragma once

#include "KrBase.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

INLINE_PROCEDURE String FmtStrV(Memory_Arena *arena, const char *fmt, va_list list)
{
    va_list args;
    va_copy(args, list);
    int   len = 1 + vsnprintf(NULL, 0, fmt, args);
    char *buf = (char *)PushSize(arena, len);
    vsnprintf(buf, len, fmt, list);
    va_end(args);
    return StringMake(buf, len - 1);
}

INLINE_PROCEDURE String FmtStr(Memory_Arena *arena, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    String string = FmtStrV(arena, fmt, args);
    va_end(args);
    return string;
}

INLINE_PROCEDURE bool IsSpace(Uint32 ch)
{
    return ch == ' ' || ch == '\f' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\v';
}

INLINE_PROCEDURE String StrTrim(String str)
{
    Int64 trim = 0;
    for (Int64 index = 0; index < str.Length; ++index)
    {
        if (IsSpace(str.Data[index]))
            trim += 1;
        else
            break;
    }

    str.Data += trim;
    str.Length -= trim;

    for (Int64 index = str.Length - 1; index >= 0; --index)
    {
        if (IsSpace(str.Data[index]))
        {
            str.Data[index] = '\0';
            str.Length -= 1;
        }
        else
            break;
    }

    return str;
}

INLINE_PROCEDURE String StrRemovePrefix(String str, Int64 count)
{
    Assert(str.Length >= count);
    str.Data += count;
    str.Length -= count;
    return str;
}

INLINE_PROCEDURE String StrRemoveSuffix(String str, Int64 count)
{
    Assert(str.Length >= count);
    str.Length -= count;
    return str;
}

INLINE_PROCEDURE Int64 StrCopy(String src, char *const dst, Int64 dst_size, Int64 count)
{
    Int64 copied = (Int64)Clamp(dst_size, src.Length, count);
    memcpy(dst, src.Data, copied);
    return copied;
}

INLINE_PROCEDURE String StrDuplicate(String src)
{
    String dst;
    dst.Data = (Uint8 *)MemoryAllocate(src.Length + 1, &ThreadContext.Allocator);
    memcpy(dst.Data, src.Data, src.Length);
    dst.Length           = src.Length;
    dst.Data[dst.Length] = 0;
    return dst;
}

INLINE_PROCEDURE String StrDuplicateArena(String src, Memory_Arena *arena)
{
    String dst;
    dst.Data = (Uint8 *)PushSize(arena, src.Length + 1);
    memcpy(dst.Data, src.Data, src.Length);
    dst.Length           = src.Length;
    dst.Data[dst.Length] = 0;
    return dst;
}

INLINE_PROCEDURE String SubStr(String str, Int64 index, Int64 count)
{
    Assert(index < str.Length);
    count = (Int64)Minimum(str.Length, count);
    return StringMake(str.Data + index, count);
}

INLINE_PROCEDURE int StrCompare(String a, String b)
{
    Int64 count = (Int64)Minimum(a.Length, b.Length);
    return memcmp(a.Data, b.Data, count);
}

INLINE_PROCEDURE int StrCompareCaseInsensitive(String a, String b)
{
    Int64 count = (Int64)Minimum(a.Length, b.Length);
    for (Int64 index = 0; index < count; ++index)
    {
        if (a.Data[index] != b.Data[index] && a.Data[index] + 32 != b.Data[index] && a.Data[index] != b.Data[index])
        {
            return a.Data[index] - b.Data[index];
        }
    }
    return 0;
}

INLINE_PROCEDURE bool StrMatch(String a, String b)
{
    if (a.Length != b.Length)
        return false;
    return StrCompare(a, b) == 0;
}

INLINE_PROCEDURE bool StrMatchCaseInsensitive(String a, String b)
{
    if (a.Length != b.Length)
        return false;
    return StrCompareCaseInsensitive(a, b) == 0;
}

INLINE_PROCEDURE bool StrStartsWith(String str, String sub)
{
    if (str.Length < sub.Length)
        return false;
    return StrCompare(StringMake(str.Data, sub.Length), sub) == 0;
}

INLINE_PROCEDURE bool StrStartsWithCaseInsensitive(String str, String sub)
{
    if (str.Length < sub.Length)
        return false;
    return StrCompareCaseInsensitive(StringMake(str.Data, sub.Length), sub) == 0;
}

INLINE_PROCEDURE bool StrStartsWithCharacter(String str, Uint8 c)
{
    return str.Length && str.Data[0] == c;
}

INLINE_PROCEDURE bool StrStartsWithCharacterCaseInsensitive(String str, Uint8 c)
{
    return str.Length && (str.Data[0] == c || str.Data[0] + 32 == c || str.Data[0] == c + 32);
}

INLINE_PROCEDURE bool StrEndsWith(String str, String sub)
{
    if (str.Length < sub.Length)
        return false;
    return StrCompare(StringMake(str.Data + str.Length - sub.Length, sub.Length), sub) == 0;
}

INLINE_PROCEDURE bool StringEndsWithCaseInsensitive(String str, String sub)
{
    if (str.Length < sub.Length)
        return false;
    return StrCompareCaseInsensitive(StringMake(str.Data + str.Length - sub.Length, sub.Length), sub) == 0;
}

INLINE_PROCEDURE bool StrEndsWithCharacter(String str, Uint8 c)
{
    return str.Length && str.Data[str.Length - 1] == c;
}

INLINE_PROCEDURE bool StrEndsWithCharacterCaseInsensitive(String str, Uint8 c)
{
    return str.Length &&
           (str.Data[str.Length - 1] == c || str.Data[str.Length - 1] + 32 == c || str.Data[str.Length - 1] == c + 32);
}

INLINE_PROCEDURE char *StrNullTerminated(char *buffer, String str)
{
    memcpy(buffer, str.Data, str.Length);
    buffer[str.Length] = 0;
    return buffer;
}

INLINE_PROCEDURE char *StrNullTerminatedArena(Memory_Arena *arena, String str)
{
    return StrNullTerminated((char *)PushSize(arena, str.Length + 1), str);
}

INLINE_PROCEDURE Int64 StrFind(String str, String key, Int64 pos)
{
    Int64 index = Clamp(0, str.Length - 1, pos);
    while (str.Length >= key.Length)
    {
        if (StrCompare(StringMake(str.Data, key.Length), key) == 0)
        {
            return index;
        }
        index += 1;
        str = StrRemovePrefix(str, 1);
    }
    return -1;
}

INLINE_PROCEDURE Int64 StrFindCharacter(String str, Uint8 key, Int64 pos)
{
    for (Int64 index = Clamp(0, str.Length - 1, pos); index < str.Length; ++index)
        if (str.Data[index] == key)
            return index;
    return -1;
}

INLINE_PROCEDURE Int64 StrReverseFind(String str, String key, Int64 pos)
{
    Int64 index = Clamp(0, str.Length - key.Length, pos);
    while (index >= 0)
    {
        if (StrCompare(StringMake(str.Data + index, key.Length), key) == 0)
            return index;
        index -= 1;
    }
    return -1;
}

INLINE_PROCEDURE Int64 StrReverseFindCharacter(String str, Uint8 key, Int64 pos)
{
    for (Int64 index = Clamp(0, str.Length - 1, pos); index >= 0; --index)
        if (str.Data[index] == key)
            return index;
    return -1;
}
