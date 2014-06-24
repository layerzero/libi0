#ifndef I0STDLIB_H
#define I0STDLIB_H

#include "stddef.h"

// input a long from STDIN
// Note: will also read in an extra char
// return immediately after reading '\0'
// do not recognize +
// result is in *pq
// return 0 on success
// -1 on end of file
long str_to_long(char* str, long maxlen, long *pnext, long *result);

#endif // I0STDLIB_H
