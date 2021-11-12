#include "KrTokenizer.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define TokenizerPow(x, y) pow(x, y)
#define TokenizerFmtMessage(buff, buff_size, fmt, arg) vsnprintf(buff, buff_size, fmt, arg)

String TokenKindString(Token_Kind kind)
{
    static const String TokenStrings[] = {
        StringExpand("error"),

        StringExpand("comment"),
        StringExpand("at"),
        StringExpand("hash"),
        StringExpand("dollar"),
        StringExpand("open_bracket"),
        StringExpand("close_bracket"),
        StringExpand("slash"),
        StringExpand("open_curly_bracket"),
        StringExpand("close_curly_bracket"),
        StringExpand("open_square_bracket"),
        StringExpand("close_square_bracket"),
        StringExpand("colon"),
        StringExpand("semicolon"),
        StringExpand("comma"),
        StringExpand("period"),
        StringExpand("question_mark"),
        StringExpand("tilde"),
        StringExpand("back_tick"),

        StringExpand("exclamation"),
        StringExpand("percent"),
        StringExpand("caret"),
        StringExpand("ampersand"),
        StringExpand("astrick"),
        StringExpand("plus"),
        StringExpand("minus"),
        StringExpand("equals"),
        StringExpand("pipe"),
        StringExpand("back_slash"),
        StringExpand("less_than"),
        StringExpand("greater_than"),

        StringExpand("compare_equals"),
        StringExpand("compare_not_equals"),
        StringExpand("compare_less_than_equals"),
        StringExpand("compare_greater_than_equals"),
        StringExpand("logical_and"),
        StringExpand("logical_or"),
        StringExpand("left_shift"),
        StringExpand("right_shit"),
        StringExpand("plus_plus"),
        StringExpand("minus_minus"),
        StringExpand("plus_equals"),
        StringExpand("minus_equals"),
        StringExpand("mul_equals"),
        StringExpand("div_equals"),
        StringExpand("mod_equals"),
        StringExpand("and_equals"),
        StringExpand("or_equals"),
        StringExpand("xor_equals"),
        StringExpand("dash_arrow"),
        StringExpand("equal_arrow"),
        StringExpand("double_colon"),

        StringExpand("double_quoted_string"),
        StringExpand("single_quoted_string"),

        StringExpand("real_literal"),
        StringExpand("signed_integer_literal"),
        StringExpand("unsigned_integer_literal"),

        StringExpand("identifier"),

        StringExpand("end_of_stream") };

    Assert(ArrayCount(TokenStrings) == _Token_Kind_Count); // Strings out of data
    return TokenStrings[kind];
}

static bool IsValidIdentifierCharacater(Uint8 ch)
{
    return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' || ch > 125;
}

static bool IsNumber(uint32_t codepoint)
{
    return codepoint >= '0' && codepoint <= '9';
}

static bool IsHexNumberUpper(uint32_t codepoint)
{
    return (codepoint >= '0' && codepoint <= '9') || (codepoint >= 'a' && codepoint <= 'f');
}

static bool IsHexNumberLower(uint32_t codepoint)
{
    return (codepoint >= '0' && codepoint <= '9') || (codepoint >= 'A' && codepoint <= 'F');
}

static bool IsBinaryNumber(uint32_t codepoint)
{
    return codepoint == '0' || codepoint == '1';
}

static bool IsWhitespace(Uint8 code)
{
    return code == ' ' || code == '\t' || code == '\v' || code == '\f';
}

static bool IsNumberSeparator(Uint8 code, uint32_t index)
{
    return (code == '_') && ((index & 1) == 0 || (index & 3) == 0 || (index & 7) == 0 || (index & 15) == 0);
}

static void TokenizerStartToken(Tokenizer *tokenizer)
{
    tokenizer->TokenBuffer.Row = tokenizer->Row;
    tokenizer->TokenBuffer.Column = tokenizer->Column;
    tokenizer->TokenBuffer.Content.Data = tokenizer->Content.Data + tokenizer->Position - sizeof(tokenizer->Lookup);
}

static void TokenizerEndToken(Tokenizer *tokenizer, Token_Kind kind)
{
    tokenizer->TokenBuffer.Kind = kind;
    Uint8 *end = tokenizer->Content.Data + tokenizer->Position - ArrayCount(tokenizer->Lookup);
    tokenizer->TokenBuffer.Content.Length = (Int64)(end - tokenizer->TokenBuffer.Content.Data);
}

static void TokenizerError(Tokenizer *tokenizer, bool finish, const char *fmt, ...)
{
    TokenizerEndToken(tokenizer, Token_Kind_Error);
    va_list arg;
    va_start(arg, fmt);
    TokenizerFmtMessage((char *)tokenizer->Buffer, sizeof(tokenizer->Buffer), fmt, arg);
    va_end(arg);
    tokenizer->Finished = finish;
}

static void TokenizerAdvance(Tokenizer *tokenizer, uint32_t count)
{
    if (!tokenizer->Finished)
    {
        for (uint32_t i = 0; i < count; ++i)
        {
            tokenizer->Column += 1;
            tokenizer->Lookup[0] = tokenizer->Lookup[1];
            tokenizer->Lookup[1] = tokenizer->Lookup[2];

            if (tokenizer->Position < tokenizer->Content.Length)
            {
                tokenizer->Lookup[2] = tokenizer->Content.Data[tokenizer->Position];
                tokenizer->Position += 1;
            }
            else
            {
                tokenizer->Lookup[2] = 0;
                if (tokenizer->Lookup[0] == 0 && tokenizer->Lookup[1] == 0 && tokenizer->Lookup[2] == 0)
                {
                    tokenizer->Finished = true;
                    return;
                }
                else
                {
                    // We update Tokenizer.Position past Tokenizer.Content.Length because
                    // in TokenizerEndToken procedure, ArrayCount(Tokenizer.Lookup) is subtracted
                    // and in order for the Length of the content of the last token to be correct,
                    // Tokenizer.Position needs to get updated past Tokenizer.Content.Length
                    tokenizer->Position += 1;
                }
            }
        }
    }
}

static bool TokenizerAdvanceNewline(Tokenizer *tokenizer)
{
    if (tokenizer->Lookup[0] == '\r' || tokenizer->Lookup[0] == '\n')
    {
        if (tokenizer->Lookup[0] == '\r' && tokenizer->Lookup[1] == '\n')
        {
            TokenizerAdvance(tokenizer, 1);
        }
        TokenizerAdvance(tokenizer, 1);
        tokenizer->Column = 1;
        tokenizer->Row += 1;
        return true;
    }
    return false;
}

static bool TokenizerAdvanceWhitespace(Tokenizer *tokenizer)
{
    if (TokenizerAdvanceNewline(tokenizer))
    {
        return true;
    }
    else if (IsWhitespace(tokenizer->Lookup[0]))
    {
        while (!tokenizer->Finished && IsWhitespace(tokenizer->Lookup[0]))
            TokenizerAdvance(tokenizer, 1);
        return true;
    }
    return false;
}

static bool TokenizerAdvanceComments(Tokenizer *tokenizer)
{
    TokenizerStartToken(tokenizer);
    if (tokenizer->CommentIdentifier)
    {
        if (tokenizer->Lookup[0] == tokenizer->CommentIdentifier)
        {
            TokenizerAdvance(tokenizer, 1);

            while (!tokenizer->Finished)
            {
                if (TokenizerAdvanceNewline(tokenizer))
                    break;
                TokenizerAdvance(tokenizer, 1);
            }

            return true;
        }
        return false;
    }
    else if (tokenizer->Lookup[0] == '/')
    {
        if (tokenizer->Lookup[1] == '/')
        {
            TokenizerAdvance(tokenizer, 2);

            // single line comment
            while (!tokenizer->Finished)
            {
                if (TokenizerAdvanceNewline(tokenizer))
                    break;
                TokenizerAdvance(tokenizer, 1);
            }

            return true;
        }
        else if (tokenizer->Lookup[1] == '*')
        {
            TokenizerAdvance(tokenizer, 2);

            Ptrsize err_row = tokenizer->Row;
            Ptrsize err_col = tokenizer->Column;

            Int64 err_offset = tokenizer->Position;

            // comment block
            Uint64 comment_block_count = 1;
            while (!tokenizer->Finished && comment_block_count != 0)
            {
                if (TokenizerAdvanceWhitespace(tokenizer))
                    continue;
                else if (tokenizer->Lookup[0] == '/' && tokenizer->Lookup[1] == '*')
                {
                    TokenizerAdvance(tokenizer, 2);
                    comment_block_count += 1;
                }
                else if (tokenizer->Lookup[0] == '*' && tokenizer->Lookup[1] == '/')
                {
                    TokenizerAdvance(tokenizer, 2);
                    comment_block_count -= 1;
                }
                else
                {
                    TokenizerAdvance(tokenizer, 1);
                }
            }

            if (comment_block_count != 0)
            {
                TokenizerError(tokenizer, false, "Comment not closed");
            }
            else if (tokenizer->TokenizeComment)
            {
                TokenizerEndToken(tokenizer, Token_Kind_Comment);
            }

            return true;
        }
    }

    return false;
}

static bool TokenizerAdvanceNonDecimalNumber(Tokenizer *tokenizer, char *buffer, uint32_t *buffer_index, const uint32_t MaxBufferSize, uint32_t separator_index, uint32_t radix, bool upper)
{
    while (true)
    {
        bool valid_digit = false;

        if (radix == 2)
            valid_digit = IsBinaryNumber(tokenizer->Lookup[0]);
        else 
        {
            Assert(radix == 16);
            valid_digit = upper ? IsHexNumberUpper(tokenizer->Lookup[0]) : IsHexNumberLower(tokenizer->Lookup[0]);
        }

        if (valid_digit)
        {
            if (*buffer_index < MaxBufferSize)
            {
                buffer[*buffer_index] = tokenizer->Lookup[0];
                *buffer_index += 1;
                separator_index += 1;
                TokenizerAdvance(tokenizer, 1);
            }
            else
            {
                TokenizerError(tokenizer, true, "Out of range for given number");
                return false;
            }
        }
        else if (IsNumberSeparator(tokenizer->Lookup[0], separator_index))
        {
            TokenizerAdvance(tokenizer, 1);
        }
    }

    return true;
}

bool TokenizerAdvanceNumber(Tokenizer *tokenizer, char *buffer, uint32_t *buffer_index, const uint32_t MaxBufferSize, uint32_t separator_index, uint32_t radix)
{
    if (radix == 10)
    {
        while (IsNumber(tokenizer->Lookup[0]))
        {
            if (*buffer_index < MaxBufferSize)
            {
                buffer[*buffer_index] = tokenizer->Lookup[0];
                *buffer_index += 1;
                TokenizerAdvance(tokenizer, 1);
            }
            else
            {
                TokenizerError(tokenizer, true, "Out of range for decimal integer");
                return false;
            }
        }
        return true;
    }
    else if (radix == 16)
    {
        if (buffer[1] == 'x')
        {
            return TokenizerAdvanceNonDecimalNumber(tokenizer, buffer, buffer_index, MaxBufferSize, separator_index, 16, false);
        }
        else
        {
            return TokenizerAdvanceNonDecimalNumber(tokenizer, buffer, buffer_index, MaxBufferSize, separator_index, 16, true);
        }
    }
    else
    {
        Assert(radix == 2);
        return TokenizerAdvanceNonDecimalNumber(tokenizer, buffer, buffer_index, MaxBufferSize, separator_index, 2, false);
    }
}

static bool TokenizerGotToken(Tokenizer *tokenizer)
{
    return tokenizer->TokenBuffer.Kind != Token_Kind_Error;
}

static void TokenizerNext(Tokenizer *tokenizer)
{
    tokenizer->TokenBuffer.Kind = Token_Kind_Error;

    while (!tokenizer->Finished)
    {
        if (TokenizerAdvanceComments(tokenizer) || TokenizerAdvanceWhitespace(tokenizer))
        {
            if (TokenizerGotToken(tokenizer))
                return;
            continue;
        }

        TokenizerStartToken(tokenizer);

        Uint8 a = tokenizer->Lookup[0];
        Uint8 b = tokenizer->Lookup[1];
        Uint8 c = tokenizer->Lookup[2];

        if (a <= 125)
        {
            if (b == '=')
            {
                switch (a)
                {
                case '=':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_Compare_Equals);
                    return;

                case '!':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_Compare_Equals);
                    return;

                case '<':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_Compare_Less_Than_Equals);
                    return;

                case '>':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_Compare_Greater_Than_Equals);
                    return;

                case '+':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_Plus_Equals);
                    return;

                case '-':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_Minus_Equals);
                    return;

                case '*':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_Mul_Equals);
                    return;

                case '/':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_Div_Equals);
                    return;

                case '%':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_Mod_Equals);
                    return;

                case '&':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_And_Equals);
                    return;

                case '|':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_Or_Equals);
                    return;

                case '^':
                    TokenizerAdvance(tokenizer, 2);
                    TokenizerEndToken(tokenizer, Token_Kind_Xor_Equals);
                    return;
                }
            }
            else if (a == '&' && b == '&')
            {
                TokenizerAdvance(tokenizer, 2);
                TokenizerEndToken(tokenizer, Token_Kind_Logical_And);
                return;
            }
            else if (a == '|' && b == '|')
            {
                TokenizerAdvance(tokenizer, 2);
                TokenizerEndToken(tokenizer, Token_Kind_Logical_Or);
                return;
            }
            else if (a == '<' && b == '<')
            {
                TokenizerAdvance(tokenizer, 2);
                TokenizerEndToken(tokenizer, Token_Kind_Left_Shift);
                return;
            }
            else if (a == '>' && b == '>')
            {
                TokenizerAdvance(tokenizer, 2);
                TokenizerEndToken(tokenizer, Token_Kind_Right_Shift);
                return;
            }
            else if (a == '+' && b == '+')
            {
                TokenizerAdvance(tokenizer, 2);
                TokenizerEndToken(tokenizer, Token_Kind_Plus_Plus);
                return;
            }
            else if (a == '-' && b == '-')
            {
                TokenizerAdvance(tokenizer, 2);
                TokenizerEndToken(tokenizer, Token_Kind_Minus_Minus);
                return;
            }
            else if (a == '-' && b == '>')
            {
                TokenizerAdvance(tokenizer, 2);
                TokenizerEndToken(tokenizer, Token_Kind_Dash_Arrow);
                return;
            }
            else if (a == '=' && b == '>')
            {
                TokenizerAdvance(tokenizer, 2);
                TokenizerEndToken(tokenizer, Token_Kind_Equal_Arrow);
                return;
            }
            else if (a == ':' && b == ':')
            {
                TokenizerAdvance(tokenizer, 2);
                TokenizerEndToken(tokenizer, Token_Kind_Double_Colon);
                return;
            }
            else
            {
                switch (a)
                {
                case '@':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_At);
                    return;

                case '#':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Hash);
                    return;

                case '$':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Dollar);
                    return;

                case '(':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Open_Bracket);
                    return;

                case ')':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Close_Bracket);
                    return;

                case '{':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Open_Curly_Bracket);
                    return;

                case '}':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Close_Curly_Bracket);
                    return;

                case '[':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Open_Square_Bracket);
                    return;

                case ']':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Close_Square_Bracket);
                    return;

                case ':':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Colon);
                    return;

                case ';':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Semicolon);
                    return;

                case ',':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Comma);
                    return;

                case '?':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Question_Mark);
                    return;

                case '~':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Tilde);
                    return;

                case '`':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Back_Tick);
                    return;

                case '!':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Exclamation);
                    return;

                case '%':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Percent);
                    return;

                case '^':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Caret);
                    return;

                case '|':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Caret);
                    return;

                case '&':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Ampersand);
                    return;

                case '*':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Astrick);
                    return;

                case '=':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Equals);
                    return;

                case '<':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Less_Than);
                    return;

                case '>':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Greater_Than);
                    return;

                case '/':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Slash);
                    return;

                case '\\':
                    TokenizerAdvance(tokenizer, 1);
                    TokenizerEndToken(tokenizer, Token_Kind_Back_Slash);
                    return;

                case '+':
                    if (!IsNumber(b) && (b != '.' || !IsNumber(c)))
                    {
                        TokenizerAdvance(tokenizer, 1);
                        TokenizerEndToken(tokenizer, Token_Kind_Plus);
                        return;
                    }
                    break;

                case '-':
                    if (!IsNumber(b) && (b != '.' || !IsNumber(c)))
                    {
                        TokenizerAdvance(tokenizer, 1);
                        TokenizerEndToken(tokenizer, Token_Kind_Minus);
                        return;
                    }
                    break;

                case '.':
                    if (!IsNumber(b))
                    {
                        TokenizerAdvance(tokenizer, 1);
                        TokenizerEndToken(tokenizer, Token_Kind_Period);
                        return;
                    }
                    break;
                }
            }

            if (a == '"' || a == '\'')
            {
                TokenizerAdvance(tokenizer, 1);

                bool end_quote_found = 0;
                while (!tokenizer->Finished)
                {
                    if (TokenizerAdvanceNewline(tokenizer))
                        break;
                    else if (tokenizer->Lookup[0] == '\\')
                    {
                        if (!TokenizerAdvanceNewline(tokenizer))
                            TokenizerAdvance(tokenizer, 2);
                        continue;
                    }
                    else if (tokenizer->Lookup[0] == a)
                    {
                        end_quote_found = true;
                        TokenizerAdvance(tokenizer, 1);
                        break;
                    }
                    else
                        TokenizerAdvance(tokenizer, 1);
                }

                if (end_quote_found)
                {
                    Token_Kind kind = a == '"' ? Token_Kind_Double_Quoted_String : Token_Kind_Single_Quoted_String;
                    TokenizerEndToken(tokenizer, kind);
                    tokenizer->TokenBuffer.Value.String.Data = tokenizer->TokenBuffer.Content.Data + 1;
                    tokenizer->TokenBuffer.Value.String.Length = tokenizer->TokenBuffer.Content.Length - 2;
                }
                else
                {
                    TokenizerError(tokenizer, true, "String terminator quote: %c ", a);
                }

                return;
            }
            else if (IsNumber(a) || a == '.' || a == '+' || a == '-')
            {
                Assert(sizeof(tokenizer->Buffer) > 2); // Buffer of at least size 2 is required

                uint32_t MaxBufferSize = sizeof(tokenizer->Buffer) - 1; // -1 for null terminator
                char *buffer = (char *)tokenizer->Buffer;
                uint32_t buffer_index = 0;

                int32_t sign = 1;
                if (tokenizer->Lookup[0] == '-')
                {
                    sign = -1;
                    TokenizerAdvance(tokenizer, 1);
                }
                else if (tokenizer->Lookup[0] == '+')
                {
                    TokenizerAdvance(tokenizer, 1);
                }

                uint32_t separator_index = 0;
                uint32_t radix = 10;
                if (tokenizer->Lookup[0] == '0')
                {
                    TokenizerAdvance(tokenizer, 1);

                    a = tokenizer->Lookup[0];
                    if (a == 'x' || a == 'X')
                    {
                        radix = 16;
                        buffer[0] = '0';
                        buffer[1] = a;
                        buffer_index = 2;
                        TokenizerAdvance(tokenizer, 1);
                    }
                    else if (a == 'b')
                    {
                        radix = 2;
                        buffer[0] = '0';
                        buffer[1] = a;
                        buffer_index = 2;
                        TokenizerAdvance(tokenizer, 1);
                    }
                    else
                    {
                        separator_index = 1;
                        while (tokenizer->Lookup[0] == '0')
                        {
                            separator_index += 1;
                            TokenizerAdvance(tokenizer, 1);
                        }
                    }
                }

                if (!TokenizerAdvanceNumber(tokenizer, buffer, &buffer_index, MaxBufferSize, separator_index, radix))
                    return;

                bool is_real = false;
                if (radix != 2)
                {
                    if (tokenizer->Lookup[0] == '.')
                    {
                        if (buffer_index < MaxBufferSize)
                        {
                            buffer[buffer_index] = tokenizer->Lookup[0];
                            buffer_index += 1;
                            is_real = true;
                            TokenizerAdvance(tokenizer, 1);
                        }
                        else
                        {
                            TokenizerError(tokenizer, true, "Out of range for given number");
                            return;
                        }
                    }

                    if (radix == 10)
                    {
                        if (tokenizer->Lookup[0] == 'e' || tokenizer->Lookup[0] == 'E')
                        {
                            if (buffer_index < MaxBufferSize)
                            {
                                buffer[buffer_index] = tokenizer->Lookup[0];
                                buffer_index += 1;
                                is_real = true;
                                TokenizerAdvance(tokenizer, 1);
                            }
                            else
                            {
                                TokenizerError(tokenizer, true, "Out of range for given number");
                                return;
                            }
                        }
                    }
                    else
                    {
                        Assert(radix = 16);
                        if (tokenizer->Lookup[0] == 'p' || tokenizer->Lookup[0] == 'P')
                        {
                            if (buffer_index < MaxBufferSize)
                            {
                                buffer[buffer_index] = tokenizer->Lookup[0];
                                buffer_index += 1;
                                is_real = true;
                                TokenizerAdvance(tokenizer, 1);
                            }
                            else
                            {
                                TokenizerError(tokenizer, true, "Out of range for given number");
                                return;
                            }
                        }
                    }

                    if (tokenizer->Lookup[0] == '+' || tokenizer->Lookup[0] == '-')
                    {
                        if (buffer_index < MaxBufferSize)
                        {
                            buffer[buffer_index] = tokenizer->Lookup[0];
                            buffer_index += 1;
                            is_real = true;
                            TokenizerAdvance(tokenizer, 1);
                        }
                        else
                        {
                            TokenizerError(tokenizer, true, "Out of range for given number");
                            return;
                        }
                    }

                    if (!TokenizerAdvanceNumber(tokenizer, buffer, &buffer_index, MaxBufferSize, 0, radix))
                        return;
                }

                Uint8 ch = tokenizer->Lookup[0];
                if ((IsWhitespace(ch) || ch == '\r' || ch == '\n') || !IsValidIdentifierCharacater(ch))
                {
                    buffer[buffer_index] = 0;

                    if (!is_real)
                    {
                        if (sign == 1)
                        {
                            TokenizerEndToken(tokenizer, Token_Kind_Unsigned_Integer_Literal);
                            tokenizer->TokenBuffer.Value.UnsignedInteger = strtoull(buffer + 2, NULL, (int)radix);
                        }
                        else
                        {
                            TokenizerEndToken(tokenizer, Token_Kind_Signed_Integer_Literal);
                            tokenizer->TokenBuffer.Value.SignedInteger = -1 * strtoll(buffer + 2, NULL, (int)radix);
                        }
                    }
                    else
                    {
                        TokenizerEndToken(tokenizer, Token_Kind_Real_Literal);
                        tokenizer->TokenBuffer.Value.Real = strtod(buffer, NULL);
                    }

                    if (errno == ERANGE)
                    {
                        TokenizerError(tokenizer, false, "Out of range for binary number");
                    }

                    return;
                }
                else
                {
                    TokenizerError(tokenizer, true, "Unexpected character. Expected number literal");
                }

                return;
            }
        }

        if (IsValidIdentifierCharacater(tokenizer->Lookup[0]))
        {
            TokenizerAdvance(tokenizer, 1);
            while (IsValidIdentifierCharacater(tokenizer->Lookup[0]))
            {
                TokenizerAdvance(tokenizer, 1);
            }
            TokenizerEndToken(tokenizer, Token_Kind_Identifier);
            return;
        }
        else
        {
            TokenizerError(tokenizer, true, "Bad character: %c ", tokenizer->Lookup[0]);
            return;
        }
    }

    TokenizerStartToken(tokenizer);
    TokenizerEndToken(tokenizer, Token_Kind_End_Of_Stream);
}

void TokenizerInit(Tokenizer *tokenizer, String content, Uint8 comment_id, bool tokenize_comment)
{
    tokenizer->Content = content;
    tokenizer->Position = 0;
    tokenizer->Lookup[0] = 0;
    tokenizer->Lookup[1] = 0;
    tokenizer->Lookup[2] = 0;

    tokenizer->TokenBuffer.Kind = Token_Kind_Error;
    tokenizer->Buffer[0] = 0;
    tokenizer->CommentIdentifier = comment_id;
    tokenizer->TokenizeComment = tokenize_comment;
    tokenizer->Finished = false;

    TokenizerAdvance(tokenizer, 3);

    tokenizer->Row = 1;
    tokenizer->Column = 1;

    TokenizerNext(tokenizer);
}

Token TokenizerNextToken(Tokenizer *tokenizer)
{
    Token token = tokenizer->TokenBuffer;
    TokenizerNext(tokenizer);
    return token;
}

Token TokenizerPeekToken(Tokenizer *tokenizer)
{
    return tokenizer->TokenBuffer;
}