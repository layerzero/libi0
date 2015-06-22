#ifndef I0MATH_H
#define I0MATH_H


long ipow(long base, long exp);
double exp(double x);
double sqrt(double x);

// simulate a fast % (modulus) operation (mathematical version)
long remainder(long dividend, long divisor);

// return the largest integral value less than or equal to x
double floor(double x);

// return the smallest integral value greater than or equal to x
double ceil(double x);

#endif // I0MATH_H
