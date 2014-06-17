#include "stddef.h"
#include "stdio.h"
#include "string.h"

// functions

// get current task's ID
long get_my_task_id()
{
    return *(long*)(RUNNER_ID);
}

// get the new tasks' ID.
// should be called just after invoking "runner" or spawn/spawnx
long get_new_task_id()
{
    return *(long*)(NEW_RUNNER_ID_ADDR);
}

// calculate the exit code range from the ID
// len in bytes
void task_id_to_ec_range(long id, long *base, long *len)
{
    *base = id;
    *len = EC_RANGE;
    return;
}

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

// NOTE: n must be 8*N, use b:sl to optimize
// Consider using memcmp which is more efficient
// TODO: use register to optimize
long strncmp(char *s1, char *s2, long n)
{
    // tn = s1 + n
    // i1 = s1
    // i2 = s2
    // while i2 < tn {
    //    br:e:sq a i1, i2, continue
    //    br:sl:sq a i1, i2, less
    //    br:j more
    //  continue:
    //    i1 = i1 + 8
    //    i2 = i2 + 8
    // }
    // goto equal
    // less:
    //    return -1;
    // more:
    //    return 1;
    // equal:
    //    return 0;

    // long a;
    // output_str(s1);
    // output_char(C_n);
    // output_str(s2);
    // output_char(C_n);

    // i1
    *(char**)0x200000020 = s1;
    // i2
    *(char**)0x200000028 = s2;
    // t
    *(char**)0x200000030 = s2 + n;

#ifdef _STRNCMP_DEBUG_
    output_str(*(char**)0x200000020);
    output_char(C_n);
    output_str(*(char**)0x200000028);
    output_char(C_n);
#endif

#ifdef _STRNCMP_DEBUG_
    output_char('t');
    output_char(':');
    output_q(*(long*)0x200000030);
    output_char(C_n);
#endif

strncmp_while:
#ifdef _STRNCMP_DEBUG_
    output_char('s');
    output_char(':');
    output_q(*(long*)0x200000028);
    output_char(C_n);
#endif
    // while i2 < tn {
    //    => if tn <= i2, jump to strncmp_e
    // asm("br:le:sq a 0x200000030, 0x200000028, $0x400001832");
    asm("br:le:sq a 0x200000030, 0x200000028, $strncmp_e");
    //    br:e:sq a i1, i2, continue
    //    => jump to strncmp_cont
    //
    // if ((*(long*)0x210000000) == (*(long*)0x210000008)) {
    //     goto strncmp_cont;
    // }
    //
#ifdef _STRNCMP_DEBUG_
    output_q(**(long**)0x200000020);
    output_char(C_n);
    output_q(**(long**)0x200000028);
    output_char(C_n);
#endif
    // asm("br:e:sq a (0x200000020), (0x200000028), $0x400001894");
    asm("br:ne:sq a (0x200000020), (0x200000028), $strncmp_ne");
    //  continue:
    //    i1 = i1 + 8
    asm("add:sq 0x200000020, $8, 0x200000020");
    //    i2 = i2 + 8
    asm("add:sq 0x200000028, $8, 0x200000028");
    //   => jump to strncmp_while
    asm("br:j a $strncmp_while");

strncmp_ne:


    //    br:sl:sq a i1, i2, less
    //    => jump to strncmp_l
    // asm("br:sl:sq a 0x200000020, 0x200000028, $0x4000017AD");
    asm("br:sl:sq a 0x200000020, 0x200000028, $strncmp_l");

    //    br:j more
    //    => jump to strncmp_g
    // asm("br:j a $0x40000174B");
    asm("br:j a $strncmp_g");

    // goto strncmp_while;

    // *(long *)(0x100000208) = '!';

strncmp_g:
#ifdef _STRNCMP_DEBUG_
    *(long *)(0x100000208) = 'g';
#endif
    return 1;

strncmp_l:
#ifdef _STRNCMP_DEBUG_
    *(long *)(0x100000208) = 'l';
#endif
    return -1;

strncmp_e:
#ifdef _STRNCMP_DEBUG_
    *(long *)(0x100000208) = '=';
#endif
    return 0;

    // just make compiler print out addresses
    goto strncmp_g;
    goto strncmp_l;
    goto strncmp_e;
    goto strncmp_while;
    goto strncmp_ne;

    // for compiler
    return;
}

void memcpy(char* src, long len, char* dst)
{
    long src_n;
    long len8;

    src_n = 0;
    if (len > 8) {
        len8 = len/8*8;
        while (src_n < len8) {
            *(long*)(&(dst[src_n])) = *(long*)(&(src[src_n]));
            src_n = src_n + 8;
        }
    }
    while (src_n < len) {
        dst[src_n] = src[src_n];
        src_n = src_n + 1;
    }

    return;
}

// for internal implementation only for memcmp_se
long __memcmp_se(char* s1, char* s2, long n)
{
    long ret_value;

    reg1 = (long)s1;
    reg2 = (long)s2;
    reg3 = n;

    asm("strcmp reg1, reg3, reg2, reg3, reg4");

    ret_value = reg4;

    return ret_value;
}

long memcmp(char* s1, char* s2, long n)
{
    long ret_value;

    long sr1;
    long sr2;
    long sr3;
    long sr4;

    // save registers
    sr1 = reg1;
    sr2 = reg2;
    sr3 = reg3;
    sr4 = reg4;

    reg1 = (long)s1;
    reg2 = (long)s2;
    reg3 = n;

    asm("strcmp reg1, reg3, reg2, reg3, reg4");

    ret_value = reg4;

    // restore registers
    reg1 = sr1;
    reg2 = sr2;
    reg3 = sr3;
    reg4 = sr4;

    return ret_value;
}

void memset0(char* m, long len)
{
    char *t;
    t = m + len;
    while(m < t) {
        *m = (char)0;
        m = m + 1;
    }
    return;
}
