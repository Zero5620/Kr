#include "KrString.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

String FormatStringVL(Memory_Allocator allocator, const char *fmt, va_list list) {
	va_list args;
	va_copy(args, list);
	int   len = 1 + vsnprintf(NULL, 0, fmt, args);
	char *buf = (char *)MemoryAllocate(len, allocator);
	vsnprintf(buf, len, fmt, list);
	va_end(args);
	return String(buf, len - 1);
}

String FormatString(Memory_Allocator allocator, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String string = FormatStringVL(allocator, fmt, args);
	va_end(args);
	return string;
}

String FormatStringVL(const char *fmt, va_list list) {
	va_list args;
	va_copy(args, list);
	int   len = 1 + vsnprintf(NULL, 0, fmt, args);
	char *buf = (char *)MemoryAllocate(len);
	vsnprintf(buf, len, fmt, list);
	va_end(args);
	return String(buf, len - 1);
}

String FormatString(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String string = FormatStringVL(fmt, args);
	va_end(args);
	return string;
}

String TmpFormatStringVL(const char *fmt, va_list list) {
	return FormatStringVL(ThreadContext.TmpMemoryAllocator, fmt, list);
}

String TmpFormatString(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String string = FormatStringVL(ThreadContext.TmpMemoryAllocator, fmt, args);
	va_end(args);
	return string;
}

bool IsEmpty(String str) {
	return str.length == 0;
}

bool IsSpace(uint32_t ch) {
	return ch == ' ' || ch == '\f' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\v';
}

String TrimString(String str) {
	ptrdiff_t trim = 0;
	for (ptrdiff_t index = 0; index < str.length; ++index) {
		if (IsSpace(str.data[index]))
			trim += 1;
		else
			break;
	}

	str.data += trim;
	str.length -= trim;

	for (ptrdiff_t index = str.length - 1; index >= 0; --index) {
		if (IsSpace(str.data[index]))
			str.length -= 1;
		else
			break;
	}

	return str;
}

String DuplicateString(String src) {
	String dst;
	dst.data = (uint8_t *)MemoryAllocate(src.length + 1);
	memcpy(dst.data, src.data, src.length);
	dst.length = src.length;
	dst.data[dst.length] = 0;
	return dst;
}

String DuplicateString(String src, Memory_Arena *arena) {
	String dst;
	dst.data = (uint8_t *)PushSize(arena, src.length + 1);
	memcpy(dst.data, src.data, src.length);
	dst.length = src.length;
	dst.data[dst.length] = 0;
	return dst;
}

String ConcatStrings(String a, String b) {
	ptrdiff_t len = a.length + b.length;
	String dst;
	dst.data = (uint8_t *)MemoryAllocate(len + 1);
	memcpy(dst.data, a.data, a.length);
	memcpy(dst.data + a.length, b.data, b.length);
	dst.length = len;
	dst.data[dst.length] = 0;
	return dst;
}

String SubString(const String str, ptrdiff_t index, ptrdiff_t count) {
	Assert(index <= str.length);
	count = (ptrdiff_t)Minimum(str.length - index, count);
	return String(str.data + index, count);
}

String SubString(const String str, ptrdiff_t index) {
	Assert(index <= str.length);
	ptrdiff_t count = str.length - index;
	return String(str.data + index, count);
}

int CompareString(String a, String b) {
	ptrdiff_t count = (ptrdiff_t)Minimum(a.length, b.length);
	return memcmp(a.data, b.data, count);
}

int CompareStringICase(String a, String b) {
	ptrdiff_t count = (ptrdiff_t)Minimum(a.length, b.length);
	for (ptrdiff_t index = 0; index < count; ++index) {
		if (a.data[index] != b.data[index] && a.data[index] + 32 != b.data[index] && a.data[index] != b.data[index] + 32) {
			return a.data[index] - b.data[index];
		}
	}
	return 0;
}

bool MatchString(String a, String b) {
	if (a.length != b.length)
		return false;
	return CompareString(a, b) == 0;
}

bool MatchStringICase(String a, String b) {
	if (a.length != b.length)
		return false;
	return CompareStringICase(a, b) == 0;
}

bool StringStartsWith(String str, String sub) {
	if (str.length < sub.length)
		return false;
	return CompareString(String(str.data, sub.length), sub) == 0;
}

bool StringStartsWithICase(String str, String sub) {
	if (str.length < sub.length)
		return false;
	return CompareStringICase(String(str.data, sub.length), sub) == 0;
}

bool StringStartsWith(String str, uint8_t c) {
	return str.length && str.data[0] == c;
}

bool StringStartsWithICase(String str, uint8_t c) {
	return str.length && (str.data[0] == c || str.data[0] + 32 == c || str.data[0] == c + 32);
}

bool StringEndsWith(String str, String sub) {
	if (str.length < sub.length)
		return false;
	return CompareString(String(str.data + str.length - sub.length, sub.length), sub) == 0;
}

bool StringEndsWithICase(String str, String sub) {
	if (str.length < sub.length)
		return false;
	return CompareStringICase(String(str.data + str.length - sub.length, sub.length), sub) == 0;
}

bool StringEndsWith(String str, uint8_t c) {
	return str.length && str.data[str.length - 1] == c;
}

bool StringEndsWithICase(String str, uint8_t c) {
	return str.length &&
		(str.data[str.length - 1] == c || str.data[str.length - 1] + 32 == c || str.data[str.length - 1] == c + 32);
}

char *TmpToNullTerminatedString(String str) {
	char *cstr = (char *)MemoryAllocate(str.length + 1, ThreadContext.TmpMemoryAllocator);
	memcpy(cstr, str.data, str.length);
	cstr[str.length] = 0;
	return cstr;
}

String RemovePrefix(String str, ptrdiff_t count) {
	Assert(str.length >= count);
	str.data += count;
	str.length -= count;
	return str;
}

String RemoveSuffix(String str, ptrdiff_t count) {
	Assert(str.length >= count);
	str.length -= count;
	return str;
}

ptrdiff_t FindString(String str, const String key, ptrdiff_t pos) {
	str = SubString(str, pos);
	while (str.length >= key.length) {
		if (CompareString(String(str.data, key.length), key) == 0) {
			return pos;
		}
		pos += 1;
		str = RemovePrefix(str, 1);
	}
	return -1;
}

ptrdiff_t FindStringICase(String str, const String key, ptrdiff_t pos) {
	str = SubString(str, pos);
	while (str.length >= key.length) {
		if (CompareString(String(str.data, key.length), key) == 0) {
			return pos;
		}
		pos += 1;
		str = RemovePrefix(str, 1);
	}
	return -1;
}

ptrdiff_t FindChar(String str, uint8_t key, ptrdiff_t pos) {
	ptrdiff_t index = Clamp(0, str.length - 1, pos);
	if (index >= 0) {
		for (; index < str.length; ++index)
			if (str.data[index] == key)
				return index;
	}
	return -1;
}

ptrdiff_t InvFindString(String str, String key, ptrdiff_t pos) {
	ptrdiff_t index = Clamp(0, str.length - key.length, pos);
	while (index >= 0) {
		if (CompareString(String(str.data + index, key.length), key) == 0)
			return index;
		index -= 1;
	}
	return -1;
}

ptrdiff_t InvFindChar(String str, uint8_t key, ptrdiff_t pos) {
	ptrdiff_t index = Clamp(0, str.length - 1, pos);
	if (index >= 0) {
		for (; index >= 0; --index)
			if (str.data[index] == key)
				return index;
	}
	return -1;
}

//
//
//

#define IsNumber(a) ((a) >= '0' && (a) <= '9')

bool ParseInt(String content, ptrdiff_t *value) {
	ptrdiff_t result = 0;
	ptrdiff_t index = 0;
	for (; index < content.length && IsNumber(content[index]); ++index)
		result = (result * 10 + (content[index] - '0'));
	*value = result;
	return index == content.length;
}

bool ParseHex(String content, ptrdiff_t *value) {
	ptrdiff_t result = 0;
	ptrdiff_t index = 0;
	for (; index < content.length; ++index) {
		ptrdiff_t digit;
		uint8_t ch = content[index];
		if (IsNumber(ch))
			digit = ch - '0';
		else if (ch >= 'a' && ch <= 'f')
			digit = 10 + (ch - 'a');
		else if (ch >= 'A' && ch <= 'F')
			digit = 10 + (ch - 'A');
		else
			break;
		result = (result * 16 + digit);
	}
	*value = result;
	return index == content.length;
}
