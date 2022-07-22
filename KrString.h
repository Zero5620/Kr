#pragma once

#include "KrCommon.h"

String FormatStringVL(Memory_Allocator allocator, const char *fmt, va_list list);
String FormatString(Memory_Allocator allocator, const char *fmt, ...);
String FormatStringVL(const char *fmt, va_list list);
String FormatString(const char *fmt, ...);
String TmpFormatStringVL(const char *fmt, va_list list);
String TmpFormatString(const char *fmt, ...);
bool   IsEmpty(String str);
bool   IsSpace(uint32_t ch);
String TrimString(String str);
String DuplicateString(String src);
String DuplicateString(String src, Memory_Arena *arena);
String ConcatStrings(String a, String b);
String SubString(const String str, ptrdiff_t index, ptrdiff_t count);
String SubString(const String str, ptrdiff_t index);
int    CompareString(String a, String b);
int    CompareStringICase(String a, String b);
bool   MatchString(String a, String b);
bool   MatchStringICase(String a, String b);
bool   StringStartsWith(String str, String sub);
bool   StringStartsWithICase(String str, String sub);
bool   StringStartsWith(String str, uint8_t c);
bool   StringStartsWithICase(String str, uint8_t c);
bool   StringEndsWith(String str, String sub);
bool   StringEndsWithICase(String str, String sub);
bool   StringEndsWith(String str, uint8_t c);
bool   StringEndsWithICase(String str, uint8_t c);
char * TmpToNullTerminatedString(String str);

String    RemovePrefix(String str, ptrdiff_t count);
String    RemoveSuffix(String str, ptrdiff_t count);
ptrdiff_t FindString(String str, const String key, ptrdiff_t pos = 0);
ptrdiff_t FindStringICase(String str, const String key, ptrdiff_t pos = 0);
ptrdiff_t FindChar(String str, uint8_t key, ptrdiff_t pos = 0);
ptrdiff_t InvFindString(String str, String key, ptrdiff_t pos);
ptrdiff_t InvFindChar(String str, uint8_t key, ptrdiff_t pos);

bool ParseInt(String content, ptrdiff_t *value);
bool ParseHex(String content, ptrdiff_t *value);
