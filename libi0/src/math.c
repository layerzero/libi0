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

double log(double x) {
  double y, ey;
  y = x;
  ey = exp(y);
  while (ey > x+1e-10) {
    y = y - 1 + x/ey;
    ey = exp(y);
  }
  return y;
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

double ceil(double x) {
  if (x<=0) {
    return (double)(long)x;
  } else if (x == (long)x) {
    return x;
  } else {
    return -1.0*ceil(-1*x)+1.0;
  }
}

double sin(double x) {
  double sign;
  double x2, x3, x5, x7, x9, x11, x13;
  sign = 1.0;

  // reduce to [0, 2pi)
  x = x - floor(x/(2*M_PI))*2*M_PI;

  // reduce to [0, pi)
  if (x>M_PI) {
    x = x - M_PI;
    sign = -1;
  }

  // reduce to [0, pi/2)
  if (x>M_PI_2) {
    x = M_PI - x;
  }

  x2 = x*x;
  x3 = x*x2;
  x5 = x3*x2;
  x7 = x5*x2;
  x9 = x7*x2;
  x11 = x9*x2;
  x13 = x11*x2;

  // Taylor expansion to x^13
  return sign*(x-x3/6+x5/120-x7/5040+x9/362880-x11/39916800+x13/6227020800);
}

double cos(double x) {
  return sin(M_PI_2-x);
}

double tan(double x) {
  return (sin(x)/cos(x));
}
