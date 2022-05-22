#pragma once

#include "KrCommon.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

INLINE_PROCEDURE String FmtStrV(Memory_Arena *arena, const char *fmt, va_list list) {
	va_list args;
	va_copy(args, list);
	int   len = 1 + vsnprintf(NULL, 0, fmt, args);
	char *buf = (char *)PushSize(arena, len);
	vsnprintf(buf, len, fmt, list);
	va_end(args);
	return String(buf, len - 1);
}

INLINE_PROCEDURE String FmtStr(Memory_Arena *arena, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String string = FmtStrV(arena, fmt, args);
	va_end(args);
	return string;
}

INLINE_PROCEDURE String FmtStrV(Memory_Allocator allocator, const char *fmt, va_list list) {
	va_list args;
	va_copy(args, list);
	int   len = 1 + vsnprintf(NULL, 0, fmt, args);
	char *buf = (char *)MemoryAllocate(len, allocator);
	vsnprintf(buf, len, fmt, list);
	va_end(args);
	return String(buf, len - 1);
}

INLINE_PROCEDURE String FmtStr(Memory_Allocator allocator, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String string = FmtStrV(allocator, fmt, args);
	va_end(args);
	return string;
}

INLINE_PROCEDURE bool StrIsEmpty(String str) {
	return str.length == 0;
}

INLINE_PROCEDURE bool IsSpace(uint32_t ch) {
	return ch == ' ' || ch == '\f' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\v';
}

INLINE_PROCEDURE String StrTrim(String str) {
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

INLINE_PROCEDURE String StrRemovePrefix(String str, ptrdiff_t count) {
	Assert(str.length >= count);
	str.data += count;
	str.length -= count;
	return str;
}

INLINE_PROCEDURE String StrRemoveSuffix(String str, ptrdiff_t count) {
	Assert(str.length >= count);
	str.length -= count;
	return str;
}

INLINE_PROCEDURE ptrdiff_t StrCopy(String src, char *const dst, ptrdiff_t dst_size, ptrdiff_t count) {
	ptrdiff_t copied = (ptrdiff_t)Clamp(dst_size, src.length, count);
	memcpy(dst, src.data, copied);
	return copied;
}

INLINE_PROCEDURE String StrDup(String src, Memory_Allocator allocator = ThreadContext.allocator) {
	String dst;
	dst.data = (uint8_t *)MemoryAllocate(src.length + 1, allocator);
	memcpy(dst.data, src.data, src.length);
	dst.length = src.length;
	dst.data[dst.length] = 0;
	return dst;
}

INLINE_PROCEDURE String StrDup(String src, Memory_Arena *arena) {
	String dst;
	dst.data = (uint8_t *)PushSize(arena, src.length + 1);
	memcpy(dst.data, src.data, src.length);
	dst.length = src.length;
	dst.data[dst.length] = 0;
	return dst;
}

INLINE_PROCEDURE String StrContat(String a, String b, Memory_Arena *arena) {
	ptrdiff_t len = a.length + b.length;
	String dst;
	dst.data = (uint8_t *)PushSize(arena, len + 1);
	memcpy(dst.data, a.data, a.length);
	memcpy(dst.data + a.length, b.data, b.length);
	dst.length = len;
	dst.data[dst.length] = 0;
	return dst;
}

INLINE_PROCEDURE String SubStr(const String str, ptrdiff_t index, ptrdiff_t count) {
	Assert(index <= str.length);
	count = (ptrdiff_t)Minimum(str.length - index, count);
	return String(str.data + index, count);
}

INLINE_PROCEDURE String SubStr(const String str, ptrdiff_t index) {
	Assert(index <= str.length);
	ptrdiff_t count = str.length - index;
	return String(str.data + index, count);
}

INLINE_PROCEDURE int StrCompare(String a, String b) {
	ptrdiff_t count = (ptrdiff_t)Minimum(a.length, b.length);
	return memcmp(a.data, b.data, count);
}

INLINE_PROCEDURE int StrCompareICase(String a, String b) {
	ptrdiff_t count = (ptrdiff_t)Minimum(a.length, b.length);
	for (ptrdiff_t index = 0; index < count; ++index) {
		if (a.data[index] != b.data[index] && a.data[index] + 32 != b.data[index] && a.data[index] != b.data[index] + 32) {
			return a.data[index] - b.data[index];
		}
	}
	return 0;
}

INLINE_PROCEDURE bool StrMatch(String a, String b) {
	if (a.length != b.length)
		return false;
	return StrCompare(a, b) == 0;
}

INLINE_PROCEDURE bool StrMatchICase(String a, String b) {
	if (a.length != b.length)
		return false;
	return StrCompareICase(a, b) == 0;
}

INLINE_PROCEDURE bool StrStartsWith(String str, String sub) {
	if (str.length < sub.length)
		return false;
	return StrCompare(String(str.data, sub.length), sub) == 0;
}

INLINE_PROCEDURE bool StrStartsWithICase(String str, String sub) {
	if (str.length < sub.length)
		return false;
	return StrCompareICase(String(str.data, sub.length), sub) == 0;
}

INLINE_PROCEDURE bool StrStartsWithChar(String str, uint8_t c) {
	return str.length && str.data[0] == c;
}

INLINE_PROCEDURE bool StrStartsWithCharICase(String str, uint8_t c) {
	return str.length && (str.data[0] == c || str.data[0] + 32 == c || str.data[0] == c + 32);
}

INLINE_PROCEDURE bool StrEndsWith(String str, String sub) {
	if (str.length < sub.length)
		return false;
	return StrCompare(String(str.data + str.length - sub.length, sub.length), sub) == 0;
}

INLINE_PROCEDURE bool StringEndsWithICase(String str, String sub) {
	if (str.length < sub.length)
		return false;
	return StrCompareICase(String(str.data + str.length - sub.length, sub.length), sub) == 0;
}

INLINE_PROCEDURE bool StrEndsWithChar(String str, uint8_t c) {
	return str.length && str.data[str.length - 1] == c;
}

INLINE_PROCEDURE bool StrEndsWithCharICase(String str, uint8_t c) {
	return str.length &&
		(str.data[str.length - 1] == c || str.data[str.length - 1] + 32 == c || str.data[str.length - 1] == c + 32);
}

INLINE_PROCEDURE char *StrNullTerminated(char *buffer, String str) {
	memcpy(buffer, str.data, str.length);
	buffer[str.length] = 0;
	return buffer;
}

INLINE_PROCEDURE char *StrNullTerminatedArena(Memory_Arena *arena, String str) {
	return StrNullTerminated((char *)PushSize(arena, str.length + 1), str);
}

INLINE_PROCEDURE ptrdiff_t StrFind(String str, const String key, ptrdiff_t pos = 0) {
	str = SubStr(str, pos);
	while (str.length >= key.length) {
		if (StrCompare(String(str.data, key.length), key) == 0) {
			return pos;
		}
		pos += 1;
		str = StrRemovePrefix(str, 1);
	}
	return -1;
}

INLINE_PROCEDURE ptrdiff_t StrFindICase(String str, const String key, ptrdiff_t pos = 0) {
	str = SubStr(str, pos);
	while (str.length >= key.length) {
		if (StrCompareICase(String(str.data, key.length), key) == 0) {
			return pos;
		}
		pos += 1;
		str = StrRemovePrefix(str, 1);
	}
	return -1;
}

INLINE_PROCEDURE ptrdiff_t StrFindChar(String str, uint8_t key, ptrdiff_t pos = 0) {
	ptrdiff_t index = Clamp(0, str.length - 1, pos);
	if (index >= 0) {
		for (; index < str.length; ++index)
			if (str.data[index] == key)
				return index;
	}
	return -1;
}

INLINE_PROCEDURE ptrdiff_t StrReverseFind(String str, String key, ptrdiff_t pos) {
	ptrdiff_t index = Clamp(0, str.length - key.length, pos);
	while (index >= 0) {
		if (StrCompare(String(str.data + index, key.length), key) == 0)
			return index;
		index -= 1;
	}
	return -1;
}

INLINE_PROCEDURE ptrdiff_t StrReverseFindChar(String str, uint8_t key, ptrdiff_t pos) {
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

INLINE_PROCEDURE bool ParseInt(String content, ptrdiff_t *value) {
	ptrdiff_t result = 0;
	ptrdiff_t index = 0;
	for (; index < content.length && IsNumber(content[index]); ++index)
		result = (result * 10 + (content[index] - '0'));
	*value = result;
	return index == content.length;
}

INLINE_PROCEDURE bool ParseHex(String content, ptrdiff_t *value) {
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

//
//
//

struct Builder {
	uint8_t *mem;
	ptrdiff_t  written;
	ptrdiff_t  cap;
	ptrdiff_t  thrown;
};

static void BuilderBegin(Builder *builder, void *mem, ptrdiff_t cap) {
	builder->mem = (uint8_t *)mem;
	builder->written = 0;
	builder->thrown = 0;
	builder->cap = cap;
}

static String BuilderEnd(Builder *builder) {
	String buffer(builder->mem, builder->written);
	return buffer;
}

static void BuilderWriteBytes(Builder *builder, void *ptr, ptrdiff_t size) {
	uint8_t *data = (uint8_t *)ptr;
	while (size > 0) {
		if (builder->written < builder->cap) {
			ptrdiff_t write_size = Minimum(size, builder->cap - builder->written);
			memcpy(builder->mem + builder->written, data, write_size);
			builder->written += write_size;
			size -= write_size;
			data += write_size;
		} else {
			builder->thrown += size;
			LogErrorEx("Builder", "Buffer full. Failed to write %d bytes", size);
			return;
		}
	}
}

static void BuilderWrite(Builder *builder, Buffer buffer) {
	BuilderWriteBytes(builder, buffer.data, buffer.length);
}

template <typename ...Args>
static void BuilderWrite(Builder *builder, Buffer buffer, Args... args) {
	BuilderWriteBytes(builder, buffer.data, buffer.length);
	BuilderWrite(builder, args...);
}

//
//
//

struct Str_Tokenizer {
	String   buffer;
	uint8_t *current;
	String token;
};

INLINE_PROCEDURE void StrTokenizerInit(Str_Tokenizer *tokenizer, String content) {
	tokenizer->buffer  = content;
	tokenizer->current = content.data;
	tokenizer->token   = String();
}

INLINE_PROCEDURE bool StrTokenize(Str_Tokenizer *tokenizer, const String delims) {
	uint8_t *last  = tokenizer->buffer.data + tokenizer->buffer.length;
	uint8_t *start = tokenizer->current;
	for (; tokenizer->current < last; ++tokenizer->current) {
		uint8_t *current = tokenizer->current;
		for (auto d : delims) {
			if (d == *current) {
				if (current - start) {
					tokenizer->token = String(start, current - start);
					tokenizer->current = current + 1;
					return true;
				}
				start = current + 1;
				tokenizer->current = start;
				break;
			}
		}
	}
	if (last - start) {
		tokenizer->token = String(start, last - start);
		return true;
	}
	return false;
}

INLINE_PROCEDURE bool StrTokenizeEnd(Str_Tokenizer *tokenizer) {
	uint8_t *start = tokenizer->current;
	uint8_t *last = tokenizer->buffer.data + tokenizer->buffer.length;
	tokenizer->token = String(start, last - start);
	return tokenizer->token.length == 0;
}
