#ifndef I0MATH_H
#define I0MATH_H

long ipow(long base, long exp)
{
    long result;
    result = 1;
    while (exp)
    {
        if ((exp & 1) == 1)
            result = result * base;
        exp = exp / 2;
        base = base * base;
    }

    return result;
}

#endif // I0MATH_H
