#include "stddef.h"
#include "stdio.h"
#include "sio.h"
#include "string.h"
#include "time.h"
#include "unistd.h"
#include "pc.h"
#include "math.h"
#include "fastexp.h"

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

// Read at most @nbyte from @sid to @addr.
// Returned value is:
// the number of bytes received,
// or -1 if an error occurred,
// or 0 when the peer has performed an orderly shutdown.
size_t sread(sid_t sid, char *addr, size_t nbyte)
{
    uint64_t n;

    // set system call id
    *(SYSCALL_ID_TYPE*) SYSCALL_ID_ADDR = (SYSCALL_ID_TYPE) SYSCALL_ID_SREAD;

    // set system call input arguments
    *(sid_t*) SYSCALL_COMM_AREA_ADDR = sid;

    // set system call input arguments
    *(size_t*) (SYSCALL_COMM_AREA_ADDR + sizeof_sid_t) = nbyte;

    // set system call input arguments
    *(char**) (SYSCALL_COMM_AREA_ADDR + 2 * sizeof_sid_t) = addr;

    // make sure all data are accessible
    // each page
    for (n = 0; n < nbyte; n = n + N_CHAR_PER_PAGE) {
    // for (n = 0; n < nbyte; n = n + 1) {
        addr[n] = '.';
    }
    // last page
    addr[nbyte - 1] = '.';

    // issue system call
    asm("int 0x80");

    // set return value

    n = *(size_t*) (SYSCALL_COMM_AREA_ADDR);

    // *addr = *(void**) (SYSCALL_COMM_AREA_ADDR + sizeof_size_t);

#ifdef _SIO_DEBUG_
    output_char('s');
    output_char('i');
    output_char('o');
    output_char(':');
    output_char('s');
    output_char('r');
    output_char(':');
    output_char(C_n);
    output_q(n);
    output_char(C_n);
    output_char_str((char*)addr, n);
    output_char(C_n);
#endif

    return n;
}

// Write at most @nbyte from @addr to @sid.
// Returned value is:
// the number of bytes written.
size_t swrite(sid_t sid, char *addr, size_t nbyte)
{
    long n;
    char c;

    // set system call id
    *(SYSCALL_ID_TYPE*) SYSCALL_ID_ADDR = (SYSCALL_ID_TYPE) SYSCALL_ID_SWRITE;

    // set system call input arguments
    *(sid_t*) SYSCALL_COMM_AREA_ADDR = sid;

    // set system call input arguments
    *(size_t*) (SYSCALL_COMM_AREA_ADDR + sizeof_sid_t) = nbyte;

    // set system call input arguments
    *(uint64_t*) (SYSCALL_COMM_AREA_ADDR + sizeof_sid_t + sizeof_size_t) = (uint64_t)addr;


#ifdef _SIO_DEBUG_
    output_char('s');
    output_char('i');
    output_char('o');
    output_char(':');
    output_char('s');
    output_char('w');
    output_char(':');
    output_char(C_n);
    output_q(nbyte);
    output_char(C_n);
    output_char_str(addr, nbyte);
    output_char(C_n);
#endif

    // make sure all data are accessible
    // each page
    for (n = 0; n < nbyte; n = n + N_CHAR_PER_PAGE) {
    // for (n = 0; n < nbyte; n = n + 1) {
        c = addr[n];
    }
    // last page
    c = addr[nbyte - 1];

    n = 0;

    // issue system call
    asm("int 0x80");

    n = *(uint64_t*)(SYSCALL_COMM_AREA_ADDR);

#ifdef _SIO_DEBUG_
    output_char('s');
    output_char('i');
    output_char('o');
    output_char(':');
    output_char('s');
    output_char('w');
    output_char(':');
    output_char('d');
    output_char('o');
    output_char('n');
    output_char('e');
    output_char(':');
    output_q(n);
    output_char(C_n);
#endif

    return n;
}

// Listen on port @port
// On success, return the sid
// On failure, return SID_INVALID
sid_t slisten(size_t port)
{
    sid_t sid;

    // set system call id
    *(SYSCALL_ID_TYPE*) SYSCALL_ID_ADDR = (SYSCALL_ID_TYPE) SYSCALL_ID_SLISTEN;

    // set system call input arguments
    *(size_t*) SYSCALL_COMM_AREA_ADDR = port;


    // issue system call
    asm("int 0x80");

    // set return value
    sid = *(sid_t*) SYSCALL_COMM_AREA_ADDR;

    return sid;
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

void output_double(double d)
{
    long n;
    long i;

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

/* gettime sets the amount time (in tv_sec seconds and tv_nsec
 * nanoseconds) since an unspecified point in the past (for example,
 * system start-up time, or the Epoch) on the VPC a task running on.
 *
 * Note: different VPCs possibly have different start point.
 */
long gettime(time_t *tv_sec, long *tv_nsec)
{
    long ret;

    // set system call id
    *(long*) SYSCALL_ID_ADDR = (long) SYSCALL_ID_GETTIME;

    // set system call input arguments: none here

    // call soft interrupt of system call
    asm("int 0x80");

    // get output value of system call
    ret = *(long*) ((long)SYSCALL_COMM_AREA_ADDR);
    *tv_sec = *(time_t*) ((long)SYSCALL_COMM_AREA_ADDR + sizeof_long);
    *tv_nsec = *(long*) ((long)SYSCALL_COMM_AREA_ADDR + sizeof_long + sizeof_time_t);

    return ret;
}

/*
 * substract time tv_sec, tv_nsec from  @tv_sec_from, @tv_nsec_from
 */
void time_subtract(time_t *tv_sec_from, long *tv_nsec_from,
        time_t tv_sec, long tv_nsec)
{
    if (*tv_nsec_from < tv_nsec) {
        *tv_sec_from = *tv_sec_from - 1;
        *tv_nsec_from = *tv_nsec_from + 1000000000L;
    }

    *tv_sec_from = *tv_sec_from - tv_sec;
    *tv_nsec_from = *tv_nsec_from - tv_nsec;

    return;
}
/*
 * add time tv_sec, tv_nsec to  @tv_sec_to, @tv_nsec_to
 */
void time_add(time_t *tv_sec_to, long *tv_nsec_to,
        time_t tv_sec, long tv_nsec)
{
    *tv_sec_to = *tv_sec_to + tv_sec;
    *tv_nsec_to = *tv_nsec_to + tv_nsec;

    if (*tv_nsec_to > 1000000000L) {
        *tv_nsec_to = *tv_nsec_to - 1000000000L;
        *tv_sec_to = *tv_sec_to + 1;
    }

    return;
}


/*
 * get current time; calculate the time difference; and update the
 * time by @tv_sec and @tv_nsec.
 *
 * return 0 on success. -1 on error.
 */
long print_time_diff_and_update(time_t *base_sec, long *base_nsec)
{
    time_t sec;
    long nsec;
    time_t tsec;
    long tnsec;
    long rt;

#ifdef _DEBUG_TIME_H_
    put4('d','p','1',C_n);
#endif

    // get current time
    rt = gettime(&sec, &nsec);

#ifdef _DEBUG_TIME_H_
    put4('d','p','2',C_n);
#endif

    tsec = sec;
    tnsec = nsec;

    if (rt == 0) {

#ifdef _DEBUG_TIME_H_
        put4('d','p','3',C_n);
#endif

        time_subtract(&tsec, &tnsec, *base_sec, *base_nsec);

        // update base
        *base_sec = sec;
        *base_nsec = nsec;

        // print tsec, tnsec
        output_q(tsec);
        putchar('.');
        output_q_digits(tnsec, 9);
    } else {
        put4('E', 'R', 'R', C_n);
        return rt;
    }

    return 0;
}

/*
 * get current time; calculate the time difference; and update the
 * time by @tv_sec and @tv_nsec with the difference.
 *
 * return 0 on success. -1 on error.
 */
long get_time_diff_and_update(time_t *base_sec, long *base_nsec)
{
    time_t sec;
    long nsec;
    long rt;

#ifdef _DEBUG_TIME_H_
    put4('d','p','1',C_n);
#endif

    // get current time
    rt = gettime(&sec, &nsec);

#ifdef _DEBUG_TIME_H_
    put4('d','p','2',C_n);
#endif

    if (rt == 0) {

#ifdef _DEBUG_TIME_H_
        put4('d','p','3',C_n);
#endif

        time_subtract(&sec, &nsec, *base_sec, *base_nsec);

        // update base
        *base_sec = sec;
        *base_nsec = nsec;
    } else {
        put4('E', 'R', 'R', C_n);
        return rt;
    }

    return 0;
}

// sleep for us useconds
void usleep(useconds_t us)
{
    // set system call id
    *(SYSCALL_ID_TYPE*) SYSCALL_ID_ADDR = (SYSCALL_ID_TYPE) SYSCALL_ID_USLEEP;

    // set system call input arguments
    *(useconds_t*) SYSCALL_COMM_AREA_ADDR = us;

    // issue system call
    asm("int 0x80");

    return;
}

// fill cache of phase phase_no to from range[addr, addr+len) to range [dst, dst+len); loaded from local NRC
int64_t pc_get(int64_t phase_no, void *addr, int64_t len, void *dst)
{
    // set system call id
    *(SYSCALL_ID_TYPE*) SYSCALL_ID_ADDR = (SYSCALL_ID_TYPE) SYSCALL_ID_PC_GET;

    // set system call input arguments
    *(int64_t*) SYSCALL_COMM_AREA_ADDR = phase_no;
    *(int64_t*)(SYSCALL_COMM_AREA_ADDR + sizeof_int64_t) = (int64_t)addr;
    *(int64_t*)(SYSCALL_COMM_AREA_ADDR + 2 * sizeof_int64_t) = (int64_t)len;
    *(int64_t*)(SYSCALL_COMM_AREA_ADDR + 3 * sizeof_int64_t) = (int64_t)dst;

    // issue system call
    asm("int 0x80");

    len = *(uint64_t*) (SYSCALL_COMM_AREA_ADDR);

    return phase_no;
}

// write local phase cache of phase phase_no from range [addr, addr+len) to local NRC
int64_t pc_write(int64_t phase_no, void *addr, int64_t len)
{
    // set system call id
    *(SYSCALL_ID_TYPE*) SYSCALL_ID_ADDR = (SYSCALL_ID_TYPE) SYSCALL_ID_PC_WRITE;

    // set system call input arguments
    *(int64_t*) SYSCALL_COMM_AREA_ADDR = phase_no;
    *(int64_t*)(SYSCALL_COMM_AREA_ADDR + sizeof_int64_t) = (int64_t)addr;
    *(int64_t*)(SYSCALL_COMM_AREA_ADDR + 2 * sizeof_int64_t) = (int64_t)len;

    // issue system call
    asm("int 0x80");

    len = *(uint64_t*) (SYSCALL_COMM_AREA_ADDR);

    return len;
}

// synchronize phasing cache of phase phase_no
// after pc_sync, all NRCs have the whole phasing cache of phase_no
bool pc_sync(int64_t phase_no)
{
    // set system call id
    *(SYSCALL_ID_TYPE*) SYSCALL_ID_ADDR = (SYSCALL_ID_TYPE) SYSCALL_ID_PC_SYNC;

    // set system call input arguments
    *(int64_t*) SYSCALL_COMM_AREA_ADDR = phase_no;

    // issue system call
    asm("int 0x80");

    phase_no = *(uint64_t*) (SYSCALL_COMM_AREA_ADDR);

    return phase_no;
}

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

double exp_remez13_05_05(double value)
{
    long n;
    double a, px, x;
    double d;

    // putchar('D');putdouble(value);wrln();

    x = value;

    /* n = round(x / log 2) */
    a = LOG2E * x + 0.5;
    // n = (int)a;
    n = (long)a;
    // n -= (a < 0);
    if (a < 0) n = n - 1;

    // putchar('D');putlong(n);wrln();

    /* x -= n * log2 */
    px = (double)n;
    x = x - px * C1;
    x = x - px * C2;

    /* Compute e^x using a polynomial approximation. */
    a = 1.61356848991757396195626449324190427883666354504187e-10;
    a = a * x;
    a = a + 2.09773502429720720042832865218235470540902290986604e-9;
    a = a * x;
    a = a + 2.50517997973487340520345973228530569065189960125308e-8;
    a = a * x;
    a = a + 2.7556973177905466877853160305636358948188615327725e-7;
    a = a * x;
    a = a + 2.7557319860870868162733628841564031959051602041504e-6;
    a = a * x;
    a = a + 2.48015878916569323181318984106104816451563207545696e-5;
    a = a * x;
    a = a + 1.98412698405602139306149764767811806786694849547848e-4;
    a = a * x;
    a = a + 1.38888888883724638191056667131613472090590184962952e-3;
    a = a * x;
    a = a + 8.333333333333743243093579329746662890641071879753e-3;
    a = a * x;
    a = a + 4.16666666666688187524214985734081300874557496783939e-2;
    a = a * x;
    a = a + 0.16666666666666665609766712753641211515446088211247;
    a = a * x;
    a = a + 0.49999999999999996637019704330727593814705189927683;
    a = a * x;
    a = a + 1.00000000000000000008007233620462705038544342452684;
    a = a * x;
    a = a + 1.0000000000000000000857966908786376708355989802095;

    // putchar('D');putdouble(a);wrln();

    // build 2^n in double.
    // d = 0;
    // n = n + 1023;
    // // u.s[3] = (unsigned short)((n << 4) & 0x7FF0);
    // n = (n * 16) & 0x7FF0;
    // // n = n << 16
    // n = n * 0x10000;
    // *((long*)&d) = n;

    n = ipow(2, n);
    d = (double)n;

    // putchar('D');putdouble(d);wrln();

    return a * d;
}

