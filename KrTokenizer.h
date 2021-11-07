#pragma once
#include "KrBase.h"

enum Token_Kind
{
    Token_Kind_Error = 0,

    Token_Kind_Comment,
    Token_Kind_At,
    Token_Kind_Hash,
    Token_Kind_Dollar,
    Token_Kind_Open_Bracket,
    Token_Kind_Close_Bracket,
    Token_Kind_Slash,
    Token_Kind_Open_Curly_Bracket,
    Token_Kind_Close_Curly_Bracket,
    Token_Kind_Open_Square_Bracket,
    Token_Kind_Close_Square_Bracket,
    Token_Kind_Colon,
    Token_Kind_Semicolon,
    Token_Kind_Comma,
    Token_Kind_Period,
    Token_Kind_Question_Mark,
    Token_Kind_Tilde,
    Token_Kind_Back_Tick,

    Token_Kind_Exclamation,
    Token_Kind_Percent,
    Token_Kind_Caret,
    Token_Kind_Ampersand,
    Token_Kind_Astrick,
    Token_Kind_Plus,
    Token_Kind_Minus,
    Token_Kind_Equals,
    Token_Kind_Pipe,
    Token_Kind_Back_Slash,
    Token_Kind_Less_Than,
    Token_Kind_Greater_Than,

    Token_Kind_Compare_Equals,
    Token_Kind_Compare_Not_Equals,
    Token_Kind_Compare_Less_Than_Equals,
    Token_Kind_Compare_Greater_Than_Equals,
    Token_Kind_Logical_And,
    Token_Kind_Logical_Or,
    Token_Kind_Left_Shift,
    Token_Kind_Right_Shift,
    Token_Kind_Plus_Plus,
    Token_Kind_Minus_Minus,
    Token_Kind_Plus_Equals,
    Token_Kind_Minus_Equals,
    Token_Kind_Mul_Equals,
    Token_Kind_Div_Equals,
    Token_Kind_Mod_Equals,
    Token_Kind_And_Equals,
    Token_Kind_Or_Equals,
    Token_Kind_Xor_Equals,
    Token_Kind_Dash_Arrow,
    Token_Kind_Equal_Arrow,
    Token_Kind_Double_Colon,

    Token_Kind_Double_Quoted_String,
    Token_Kind_Single_Quoted_String,

    Token_Kind_Real_Literal,
    Token_Kind_Signed_Integer_Literal,
    Token_Kind_Unsigned_Integer_Literal,

    Token_Kind_Identifier,

    Token_Kind_End_Of_Stream,

    _Token_Kind_Count
};

String TokenKindString(Token_Kind kind);

union Token_Value
{
    double Real;
    Int64 SignedInteger;
    Uint64 UnsignedInteger;
    struct
    {
        Uint64 Length;
        Uint8 *Data;
    } String;
};

struct Token
{
    Token_Kind Kind;
    String Content;
    Ptrsize Row;
    Ptrsize Column;
    Token_Value Value;
};

struct Tokenizer
{
    String Content;
    Int64 Position;
    Ptrsize Row;
    Ptrsize Column;
    Uint8 Lookup[3];

    Token TokenBuffer;
    Uint8 Buffer[256];
    Uint8 CommentIdentifier;
    bool TokenizeComment;
    bool Finished;
};

void TokenizerInit(Tokenizer *tokenizer, String content, Uint8 comment_id = 0, bool tokenize_comment = false);
Token TokenizerNextToken(Tokenizer *tokenizer);
Token TokenizerPeekToken(Tokenizer *tokenizer);
const char *TokenizerErrorMessage(Tokenizer *tokenizer) { return (char *)tokenizer->Buffer; }
