#include "KrBase.h"
#include "KrBaseCRT.h"
#include "KrMath.h"
#include "KrTokenizer.h"

#include <stdio.h>
#include <stdlib.h>

String ReadEntireFile(const char *file)
{
    FILE *f = fopen(file, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    Uint8 *string = (Uint8 *)malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);

    string[fsize] = 0;
    return String(string, (Int64)fsize);
}

void PrintToken(Token *token)
{
    const String token_str = TokenKindString(token->Kind);
    printf("%3zu:%3zu :: %-20s %.*s\n", token->Row, token->Column,
           token_str.Data, (int)token->Content.Length, token->Content.Data);
}

int main()
{
    InitThreadContextCrt(MegaBytes(64));

    String content = ReadEntireFile("Example.cpp");
    Tokenizer tokenizer;
    TokenizerInit(&tokenizer, content);

    while (true)
    {
        Token *token = TokenizerGetToken(&tokenizer);
        if (token->Kind != Token_Kind_Error)
            PrintToken(token);
        else
            printf("%3zu:%3zu :: Error: %s\n", token->Row, token->Column, TokenizerErrorMessage(&tokenizer));

        if (token->Kind == Token_Kind_End_Of_Stream)
            break;
    }

    return 0;
}

#include "KrBase.c"
#include "KrBaseCRT.c"
#include "KrMath.cpp"
#include "KrTokenizer.cpp"
