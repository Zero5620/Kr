#pragma once
#include "KrBase.h"

bool IsNumber(uint32_t codepoint);
bool IsHexNumber(uint32_t codepoint);
bool IsOctNumber(uint32_t codepoint);
bool IsBinaryNumber(uint32_t codepoint);

struct Parsed_Unsigned_Int
{
    bool Parsed;
    Uint64 UnsignedInt;
};
Parsed_Unsigned_Int ParseUnsignedInt(const String string);

struct Parsed_Signed_Int
{
    bool Parsed;
    Int64 SignedInt;
};
Parsed_Signed_Int ParseSignedInt(const String string);

struct Parsed_Real
{
    bool Parsed;
    double Real;
};
Parsed_Real ParseReal(const String string);
