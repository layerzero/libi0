#ifndef I0MATH_H
#define I0MATH_H

// Constants: pi and pi/2
#define M_PI 3.1415926535897932384626433832795
#define M_PI_2 1.5707963267948966192313216916398

long ipow(long base, long exp);
double exp(double x);
double log(double x);
double sqrt(double x);

// simulate a fast % (modulus) operation (mathematical version)
long remainder(long dividend, long divisor);

// return the largest integral value less than or equal to x
double floor(double x);

// return the smallest integral value greater than or equal to x
double ceil(double x);

// Trigonometric functions:
double sin(double x);
double cos(double x);
double tan(double x);

#endif // I0MATH_H
