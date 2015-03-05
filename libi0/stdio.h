#ifndef I0STDIO_H
#define I0STDIO_H

#include "stddef.h"
#include "stdlib.h"
#include "math.h"

#define _DEBUG_I0STDIO_no

// End of file character.
#ifndef EOF
# define EOF (-1)
#endif

// print a char to STDOUT
void output_char(long c);

// print a char to STDOUT
void output_readable_char(long c);

#define putchar(c) output_char(c)
#define putdouble(a) output_double(a)
#define putlong(a) output_long(a)

#define putq(a) output_q(a)

#define put1(c) putchar(c)
#define put2(a,b) {put1(a); put1(b);}
#define put4(a,b,c,d) {put2(a,b); put2(c,d);}
#define put8(a,b,c,d,e,f,g,h) {put4(a,b,c,d); put4(e,f,g,h);}
#define writeln4(a,b,c,d) {put4(a,b,c,d); put1(C_n);}
#define writeln6(a,b,c,d,e,f) {put4(a,b,c,d); put2(e,f); put1(C_n);}
#define writelns6(a) {writeln6(a[0],a[1],a[2],a[3],a[4],a[5]);}
#define writeln3(a,b,c) {put2(a,b); put2(c, C_n);}
#define writelns4(a) {put4(a[0], a[1], a[2], a[3]); put1(C_n);}
#define wr5cqn(a,b,c,d,e,q) {put4(a,b,c,d);output_q(q);put1(C_n);}
#define wr4n writeln4
#define wr8n(a,b,c,d,e,f,g,h) {put4(a,b,c,d); put4(e,f,g,h);put1(C_n);}
#define wrln() {putchar(C_n);}

#define output_long(a) output_q(a)
#define output_long_hex(a) output_q_hex(a)

// print a 64bit integer to STDOUT
void output_q(long n);

// print a 64bit integer of at least d digits (filled 0s for padding)
// to STDOUT
void output_q_digits(long n, long d);

// print a 64bit integer in hexdecimal format to STDOUT
// Note: only print 15 bits
void output_q_hex(long n);

// print the current runner's to STDOUT
void output_id();

// print @sl charactores in @s to STDOUT
void output_char_str(char* s, long sl);

void output_readable_char_str(char* s, long sl);

// print string @s to STDOUT
void output_str(char* s);

// input a char from STDIN
#define __input_char() *(long*)0x100000200

// // for debug only
// long __input_char();

#define getc1 input_char

// input a char from STDIN
char input_char();

// input a long from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
long input_q();

// -1 reach EOF
// 0 success
long input_line(char *buf, long maxlen, long *len);


// input a long from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
// result is in *pq
// return 0 on success
// -1 on end of file
long input_long(long *result);

#define IEEE754_SIGN   0x8000000000000000
#define IEEE754_EXP    0x7FF0000000000000
#define IEEE754_FRAC   0x000FFFFFFFFFFFFF
#define IEEE754_2POW52 0x0010000000000000

// N x 2 ^ M
void output_double_binary(double d);

void output_double(double d);

// input a double from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
// must leading with 0.xxx or nnn.xxx
// that is, .xxx is not supported
// result is in *result
// return 0 on success
// -1 on end of file
long input_double(double *result);

void output_double(double d);

// input a double from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
// must leading with 0.xxx or nnn.xxx
// that is, .xxx is not supported
// result is in *result
// return 0 on success
// -1 on end of file
long input_double(double *result);

#endif // I0STDIO_H
