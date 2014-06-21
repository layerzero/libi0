#ifndef I0STRING_H
#define I0STRING_H

#include "stddef.h"

// NOTE: n must be 8*N, use b:sl to optimize
// Consider using memcmp which is more efficient
// TODO: use register to optimize
long strncmp(char *s1, char *s2, long n);

void memcpy(char* src, long len, char* dst);

// for internal implementation only for memcmp_se
long __memcmp_se(char* s1, char* s2, long n);

// To use this function, the program should define 
// ENABLE_MEMCMP_SE
// before #include "libi0/string.h"; otherwise, the compilation fails.
// memcmp with side effect: registers are changed
// faster but need to save the registers by the program
#ifdef ENABLE_MEMCMP_SE
#define memcmp_se(s1, s2, n) __memcmp_se(s1, s2, n)
#else
#define memcmp_se(s1, s2, n) _Pragma ("GCC error \"To use memcmp_se, you must define ENABLE_MEMCMP_SE to indicate that you are aware its side effects.\"")
#endif

long memcmp(char* s1, char* s2, long n);

void memset0(char* m, long len);

#endif // I0STRING_H
