#include "KrBase.h"
#include "KrBaseCRT.h"
#include "KrMath.h"

#include <stdio.h>

int main()
{
    InitThreadContextCrt(MegaBytes(64));

    Vec3 a(5, 4, 2);
    Vec3 b(4, 5, 6);
    Vec3 c = a + b;

    printf("a: " Fmt_Vec3 " + b: " Fmt_Vec3 " = c:" Fmt_Vec3 "\n", ExpandVec3(a), ExpandVec3(b), ExpandVec3(c));

    return 0;
}

#include "KrBase.c"
#include "KrBaseCRT.c"
#include "KrMath.cpp"
