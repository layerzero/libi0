#ifndef I0STDLIB_H
#define I0STDLIB_H

#include "stddef.h"

#define option_block_t char*

// will trigger compiler bug if written like this
// #define option_block_t (char*)

long str_to_long(char* str, long maxlen, long *pnext, long *result);

long systemEx(char *command, option_block_t option);

long system(char *command);

#endif // I0STDLIB_H
