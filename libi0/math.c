#include "stddef.h"
#include "math.h"

long ipow(long base, long exp)
{
    long result;
    result = 1;
    while (exp) {
        if ((exp & 1) == 1)
            result = result * base;
        exp = exp / 2;
        base = base * base;
    }

    return result;
}

double exp(double x)
{
    long sreg1;
    long sreg2;
    long ret;

    sreg1 = reg1;
    sreg2 = reg2;

    reg1 = *(long*)&x;
    asm(exp:e,fd reg1, reg2);
    *(long*)&ret = reg2;

    reg1 = sreg1;
    reg2 = sreg2;

    return ret;
}
