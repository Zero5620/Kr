#include "KrTokenizer.h"

#include <math.h>
#define TokenizerPow(x, y) pow(x, y)

bool IsNumber(uint32_t codepoint)
{
    return codepoint >= '0' && codepoint <= '9';
}

bool IsHexNumber(uint32_t codepoint)
{
    return (codepoint >= '0' && codepoint <= '9') || (codepoint >= 'a' && codepoint <= 'f') || (codepoint >= 'A' && codepoint <= 'F');
}

bool IsOctNumber(uint32_t codepoint)
{
    return (codepoint >= '0' && codepoint <= '7');
}

bool IsBinaryNumber(uint32_t codepoint)
{
    return codepoint == '0' || codepoint == '1';
}

Parsed_Unsigned_Int ParseUnsignedInt(const String string)
{
    Parsed_Unsigned_Int result;

    if (!string.Length)
    {
        result.Parsed = false;
        return result;
    }

    Int64 index = 0;
    uint32_t base = 10;

    if (string[index] == '0')
    {
        index += 1;

        if (index < string.Length)
        {
            switch (string[index])
            {
            case 'x':
                index += 1;
                base = 16;
                break;
            case 'h':
                index += 1;
                base = 16;
                break;
            case 'd':
                index += 1;
                base = 10;
                break;
            case 'o':
                index += 1;
                base = 8;
                break;
            case 'b':
                index += 1;
                base = 2;
                break;
            }
        }
    }

    uint64_t value = 0;
    uint8_t code;
    for (; index < string.Length; index += 1)
    {
        code = string[index];

        if (IsNumber(code))
        {
            if ((base == 10 || base == 16) ||
                (IsOctNumber(code) && base == 8) ||
                (IsBinaryNumber(code) && base == 2))
                value = value * base + (code - '0');
            else
                break;
        }
        else if (base == 16)
        {
            if (code >= 'a' && code <= 'f')
            {
                value = value * base + (code - 'a') + 10;
            }
            else if (code >= 'A' && code <= 'F')
            {
                value = value * base + (code - 'A') + 10;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    result.Parsed = index >= string.Length;
    result.UnsignedInt = value;

    return result;
}

Parsed_Signed_Int ParseSignedInt(const String string)
{
    Parsed_Signed_Int result;

    if (!string.Length)
    {
        result.Parsed = false;
        return result;
    }

    int64_t index = 0;

    uint8_t code = string[index];
    int64_t sign = code == '-' ? -1 : 1;

    if (code == '-' || code == '+')
        index += 1;

    if (index >= string.Length)
    {
        result.Parsed = false;
        return result;
    }

    uint32_t base = 10;

    if (string[index] == '0')
    {
        index += 1;

        if (index < string.Length)
        {
            switch (string[index])
            {
            case 'x':
                index += 1;
                base = 16;
                break;
            case 'h':
                index += 1;
                base = 16;
                break;
            case 'd':
                index += 1;
                base = 10;
                break;
            case 'o':
                index += 1;
                base = 8;
                break;
            case 'b':
                index += 1;
                base = 2;
                break;
            }
        }
    }

    int64_t value = 0;
    int32_t one = 0;

    for (; index < string.Length; index += 1)
    {
        code = string[index];

        if (IsNumber(code))
        {
            if ((base == 10 || base == 16) ||
                (IsOctNumber(code) && base == 8) ||
                (IsBinaryNumber(code) && base == 2))
                value = value * base + (code - '0');
            else
                break;
        }
        else if (base == 16)
        {
            if (code >= 'a' && code <= 'f')
            {
                value = value * base + (code - 'a') + 10;
            }
            else if (code >= 'A' && code <= 'F')
            {
                value = value * base + (code - 'A') + 10;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    result.Parsed = index >= string.Length;
    result.SignedInt = sign * value;
    return result;
}

Parsed_Real ParseReal(const String string)
{
    Parsed_Real result;
    if (!string.Length)
    {
        result.Parsed = false;
        return result;
    }

    int64_t index = 0;

    uint8_t code = string[index];
    double sign = code == '-' ? -1 : 1;
    
    if (code == '-' || code == '+')
        index += 1;

    bool invalid_num = (index >= string.Length);

    double value = 0;

    for (; index < string.Length; index += 1)
    {
        code = string[index];

        if (code >= '0' && code <= '9')
            value = value * 10 + (code - '0');
        else
            break;
    }

    bool period_fail = false;
    if (index < string.Length && code == '.')
    {
        double p = 1, s = 0;
        uint32_t loop_count = 0;

        for (index += 1; index < string.Length; index += 1)
        {
            code = string[index];
            if (code >= '0' && code <= '9')
                s = s * 10 + (code - '0');
            else
                break;
            p *= 10;
            loop_count += 1;
        }

        value += s / p;
        period_fail = (loop_count == 0);
    }

    bool exponent_fail = false;
    if (index < string.Length && (code == 'e' || code == 'E'))
    {
        index += 1;

        if (index < string.Length)
        {
            code = string[index];
            bool negative = (code == '-');

            if (code == '-' || code == '+')
                index += 1;

            exponent_fail = (index >= string.Length);

            double exp = 0;
            for (; index < string.Length; index += 1)
            {
                code = string[index];
                if (code >= '0' && code <= '9')
                    exp = exp * 10 + (code - '0');
                else
                    break;
            }

            double p = TokenizerPow(10.0, exp);

            if (negative)
                value /= p;
            else
                value *= p;
        }
        else
        {
            exponent_fail = true;
        }
    }

    if (index < string.Length && (code == 'f' || code == 'F'))
        index += 1;

    result.Real = sign * value;
    result.Parsed = !(invalid_num || period_fail || exponent_fail || (index < string.Length));
    return result;
}
