#include "KrBase.h"
#include "KrBaseCRT.h"
#include "KrMath.h"
#include "KrTokenizer.h"

#include <stdio.h>

int main()
{
    InitThreadContextCrt(MegaBytes(64));

    Vec3 a = V3(5, 4, 2);
    Vec3 b = V3(4, 5, 6);
    Vec3 c = a + b;

    printf("a: " FmtV3 " + b: " FmtV3 " = c:" FmtV3 "\n", ExpandV3(a), ExpandV3(b), ExpandV3(c));

    return 0;
}

#include "KrBase.c"
#include "KrBaseCRT.c"
#include "KrMath.cpp"
#include "KrTokenizer.cpp"
