#include "stdio.h"

// print a char to STDOUT
void output_char(long c)
{
    *(long *)(0x100000208) = (long)c;
    return;
}

// print a 64bit integer to STDOUT
void output_q(long n)
{
    long num;
    long c;
    long num2;
    long count;

    num = n;
    if (n < 0) {
       num = 0 - n;
       output_char((long)'-');
    }

    num2 = 0;

    count = 0;
output_q_j1:
    c = num - (num / 10)*10;
    num = num / 10;
    num2 = num2 * 10;
    num2 = num2 + c;
    count = count + 1;
    if (num > 0) {
        goto output_q_j1;
    }

    num = 0;
output_q_j2:
    // output num by byte
    c = num2 - (num2 / 10)*10;
    num2 = num2 / 10;
    c = c + 48;
    output_char((long)c);
    num = num + 1;
    if (num < count) {
        goto output_q_j2;
    }

    return;
}

// print a 64bit integer of at least d digits (filled 0s for padding)
// to STDOUT
void output_q_digits(long n, long d)
{
    long num;
    long c;
    long num2;
    long count;

    num = n;
    if (n < 0) {
       num = 0 - n;
       output_char((long)'-');
    }

    num2 = 0;

    count = 0;
    while (num > 0) {
        c = num - (num / 10)*10;
        num = num / 10;
        num2 = num2 * 10;
        num2 = num2 + c;
        count = count + 1;
    }

    num = 0;

    while (num + count < d) {
        output_char('0');
        num = num + 1;
    }

    num = 0;
    while (num < count) {
        // output num by byte
        c = num2 - (num2 / 10)*10;
        num2 = num2 / 10;
        c = c + 48;
        output_char((long)c);
        num = num + 1;
    }

    return;
}

// print a 64bit integer in hexdecimal format to STDOUT
// Note: only print 15 bits
void output_q_hex(long n)
{
    long num;
    long c;
    long num2;

    long i;

    num = n;
    num2 = n;

    output_char((long)'0');
    output_char((long)'x');

    num2 = 0;
    i = 0;
output_q_hex_j1:
    c = num & 0xF;

    num = num / 16;
    num2 = num2 * 16;
    num2 = num2 + c;

#ifdef _DEBUG_I0STDIO_
    if (c < 10) {
        c = c + 48;
    } else {
        c = c + 55;
    }
    output_char((long)c);
#endif

    i = i + 1;
    if (i < 15) {
        goto output_q_hex_j1;
    }

#ifdef _DEBUG_I0STDIO_
    output_char(C_n);
    output_q(num2);
    output_char(C_n);
#endif

    i = 0;
output_q_hex_j2:
    // output num by byte
    c = num2 & 0xF;
    num2 = num2 / 16;
    if (c < 10) {
        c = c + 48;
    } else {
        c = c + 55;
    }
    output_char((long)c);
    i = i + 1;
    if (i < 15) {
        goto output_q_hex_j2;
    }

    return;
}

// print the current runner's to STDOUT
void output_id()
{
    long id;
    id = *(long*)0x100000120;
    output_q_hex(id);
    return;
}

// print @sl charactores in @s to STDOUT
void output_char_str(char* s, long sl)
{
    long n;
    n = 0;
    while (n < sl) {
        output_char((long)s[n]);
        n = n + 1;
    }

    return;
}

// print string @s to STDOUT
void output_str(char* s)
{
    long n;
    n = 0;
output_str_j1:
    if (s[n] != 0) {
        output_char((long)s[n]);
        n = n + 1;
        goto output_str_j1;
    }

    /*
    if (n == 0) {
        output_char((long)'N');
        output_char((long)'U');
        output_char((long)'L');
        output_char((long)'L');
    }
    */

    return;
}

// // for debug only
// long __input_char() {
//     long q;
//     q = *(long*)0x100000200;
//     output_char(':');
//     output_q(q);
//     output_char(':');
//     output_char(q);
//     output_char(C_n);
//     return q;
// }

// input a char from STDIN
char input_char()
{
    long n;

    n = *(long*)0x100000200;

    return (char)n;
}

// input a long from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
long input_q()
{
    long c;
    long is_neg;
    long is_checking;
    long n;

    n = 0;
    is_neg = 0;
    is_checking = 1;

    // try to get -
    do {
        c = __input_char();
        if (c == EOF) goto input_q_exit;
        if (c == (long)'-') {
            c = __input_char();
            if (c <= (long)'9') {
                if (c >= (long)'0') {
                    is_neg = 1;
                    is_checking = 0;
                }
            }
        } else if (c <= (long)'9') {
            if (c >= (long)'0') {
                is_checking = 0;
            }
        }
    } while (is_checking == 1);

    is_checking = 1;
    // already one digit in c
    do {
        n = n * 10 + (long)c - (long)'0';
        // read next char
        c = __input_char();
        if (c == EOF) goto input_q_exit;
        if (c > (long)'9') {
            is_checking = 0;
        }
        if (c < (long)'0') {
            is_checking = 0;
        }
    } while (is_checking == 1);

input_q_exit:
    if (is_neg == 1) {
        n = 0 - n;
    }

    return n;
}

// -1 reach EOF
// 0 success
long input_line(char *buf, long maxlen, long *len)
{
    long n;
    long ret;
    long c;

    ret = 0;

    maxlen = maxlen - 1; // leave space for '\0'
    n = 0;

    do {
        c = __input_char();
        if (c == EOF) {
            ret = -1;
            goto input_line_exit;
        }
        if (c == C_n) goto input_line_exit;
        buf[n] = (char)c;
        n = n + 1;
    } while (n < maxlen);

input_line_exit:
    buf[n] = (char)C_0;
    *len = n;
    return ret;
}

// input a long from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
// result is in *pq
// return 0 on success
// -1 on end of file
long input_long(long *result)
{
    long c;
    long is_neg;
    long is_checking;
    long n;
    long ret;

    n = 0;
    is_neg = 0;
    is_checking = 1;
    ret = -1;

    // try to get -
    do {
        c = __input_char();
        if (c == EOF) goto input_long_exit;
        if (c == (long)'-') {
            c = __input_char();
            if (c <= (long)'9') {
                if (c >= (long)'0') {
                    is_neg = 1;
                    is_checking = 0;
                    ret = 0;
                }
            }
        } else if (c <= (long)'9') {
            if (c >= (long)'0') {
                is_checking = 0;
                ret = 0;
            }
        }
    } while (is_checking == 1);

    is_checking = 1;
    // already one digit in c
    do {
        n = n * 10 + (long)c - (long)'0';
        // read next char
        c = __input_char();
        if (c == EOF) goto input_long_exit;
        if (c > (long)'9') {
            is_checking = 0;
        }
        if (c < (long)'0') {
            is_checking = 0;
        }
    } while (is_checking == 1);

input_long_exit:
    if (is_neg == 1) {
        n = 0 - n;
    }

    *result = n;
    return ret;
}

// N x 2 ^ M
void output_double_binary(double d)
{
    long exp;
    long exp_raw;
    long frac;
    long frac_raw;
    long sign;
    long lv;

    lv = *(long*)(&d);

    sign = IEEE754_SIGN & lv;
    frac_raw = (IEEE754_FRAC & lv);
    frac = frac_raw + IEEE754_2POW52;

    exp_raw = IEEE754_EXP & lv;
    exp_raw = exp_raw / IEEE754_2POW52; // better sith shift; compiler limitation
    exp = exp_raw - 1023 - 52;

    // handle 0
    if (exp_raw == 0) {
        if (frac_raw == 0) {
            putchar('0');
            return;
        }
    }

    // hanlde Inf and NaNs
    if (exp_raw == 0x7FF) {
        if (frac_raw == 0) {
            putchar('I'); putchar('n'); putchar('f');
            return;
        } else {
            putchar('N'); putchar('a'); putchar('N'); putchar('s');
            return;
        }
    }

    if (sign == IEEE754_SIGN) {
        putchar('-');
    }

    output_q(frac);

    putchar('L'); putchar('*'); putchar('2'); putchar('*'); putchar('*');

    output_q(exp);

    return;
}

void output_double(double d)
{
    long n;
    long i;

    double d1;

    if (d < 0.0) {
        d1 = 0.0 - d;
    } else {
        d1 = d;
    }

    if (d1 > 9.22E18) {
        output_double_binary(d);
        return;
    }

    if (d1 < 0.0000001) {
        output_double_binary(d);
        return;
    }

    if (d < 0.0) {
        d = 0.0 - d;
        putchar('-');
    }

    n = (long)d;
    putq(n);
    d = d - (double)n;

    putchar('.');

    for (i = 0; i < 16; i = i + 1) {
        d = d * 10.0;
        n = (long)d;
        putq(n);
        d = d - (double)n;
    }

    return;
}

// input a double from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
// must leading with 0.xxx or nnn.xxx
// that is, .xxx is not supported
// result is in *result
// return 0 on success
// -1 on end of file
long input_double(double *result)
{
    long c;
    long is_neg;
    long is_checking;
    long n;
    double d;
    double scale;

    long rt;
    long ret;
    long e;

    n = 0;
    d = 0.0;
    is_neg = 0;
    is_checking = 1;
    ret = -1;
    e = 0;

    // try to get -
    do {
        c = __input_char();
        if (c == EOF) goto input_double_exit;
        if (c == (long)'-') {
            c = __input_char();
            if (c <= (long)'9') {
                if (c >= (long)'0') {
                    is_neg = 1;
                    is_checking = 0;
                    ret = 0;
                }
            }
        } else if (c <= (long)'9') {
            if (c >= (long)'0') {
                is_checking = 0;
                ret = 0;
            }
        }
    } while (is_checking == 1);

    is_checking = 1;
    // already one digit in c
    // read the integer part
    do {
        n = n * 10 + (long)c - (long)'0';
        // read next char
        c = __input_char();
        if (c == EOF) goto input_double_exit;
        if (c == (long)'.') goto input_double_after_dot;
        if (c > (long)'9') {
            is_checking = 0;
        }
        if (c < (long)'0') {
            is_checking = 0;
        }
    } while (is_checking == 1);
    // if there is '.', it is jumpted before this
    goto input_double_exit;

input_double_after_dot:
    is_checking = 1;
    scale = 0.1;
    do {
        // read next char
        c = __input_char();
        if (c == EOF) goto input_double_exit;
        if (c <= (long)'9') {
            if (c >= (long)'0') {
                c = (long)c - (long)'0';
                d = d + scale * (double)c;
                scale = scale / 10.0;
            } else {
                is_checking = 0;
            }
        } else {
            is_checking = 0;
        }
    } while (is_checking == 1);

input_double_exit:
    d = d + (double)n;

    if (c == 'E') {
        rt = input_long(&e);
        if (rt != 0) e = 0;
    } if (c == 'e') {
        rt = input_long(&e);
        if (rt != 0) e = 0;
    }

    if (e > 0) {
        // d = d * 10^e
        d = d * (double)(ipow(10, e));
    } else if (e < 0) {
        d = d / (double)(ipow(10, e));
    }

    if (is_neg == 1) {
        d = 0.0 - d;
    }

    *result = d;
    return ret;
}
