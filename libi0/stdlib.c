#include "stdlib.h"

// input a long from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
// result is in *pq
// return 0 on success
// -1 on end of file
long str_to_long(char* str, long maxlen, long *pnext, long *result)
{
    long c;
    long is_neg;
    long is_checking;
    long n;
    long ret;
    long next;

    n = 0;
    is_neg = 0;
    is_checking = 1;
    ret = -1;
    next = 0;

    // try to get -
    do {
        if (next < maxlen) {
            c = (long)str[next];
            next = next + 1;
        } else {
            goto str_to_long_exit;
        }

        if (c == C_0) goto str_to_long_exit;
        if (c == (long)'-') {
            if (next < maxlen) {
                c = (long)str[next];
                next = next + 1;
            } else {
                goto str_to_long_exit;
            }

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
        if (next < maxlen) {
            c = (long)str[next];
            next = next + 1;
        } else {
            goto str_to_long_exit;
        }

        if (c == C_0) goto str_to_long_exit;
        if (c > (long)'9') {
            is_checking = 0;
        }
        if (c < (long)'0') {
            is_checking = 0;
        }
    } while (is_checking == 1);

str_to_long_exit:
    if (is_neg == 1) {
        n = 0 - n;
    }

    *pnext = next;
    *result = n;
    return ret;
}

