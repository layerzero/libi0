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
    asm("exp:e,fd reg1, reg2");
    *(long*)&ret = reg2;

    reg1 = sreg1;
    reg2 = sreg2;

    return ret;
}

double sqrt(double x)
{
    double n;
    double y;
    n = x / 2.0;
    y = 0.0;
    while(n != y) {
        y = n;
        n = (n + x/n) / 2.0;
    }
    return n;
}

long remainder(long dividend, long divisor) {
	long res;
	res = dividend/divisor;
	res = dividend - res*divisor;
	if(res < 0) {
		res = res + divisor;
	}
	return res;
}

double floor(double x) {
  if (x>=0) {
    return (double)(long)x;
  } else if (x == (long)x) {
    return x;
  } else {
    return -1.0*floor(-1*x)-1.0;
  }
}

