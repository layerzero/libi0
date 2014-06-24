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

