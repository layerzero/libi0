/* Declaration of Puc runtime functions
   Copyright 2014-15, Hututa Technologies */

#ifndef I0STDLIB_H
#define I0STDLIB_H

#include "stddef.h"

#define option_block_t char*

// will trigger compiler bug if written like this
// #define option_block_t (char*)

long str_to_long(char* str, long maxlen, long *pnext, long *result);

/* Apparently, invoke command with option and wait till the command exits.
   Return the return value of the SYSTEMEX system call.  20150910/gl */
long systemEx(char *command, option_block_t option);

/* Invoke command without options.
   Apparently, return the return value of the SYSTEMEX system call. 20150910/gl. */
long system(char *command);

#endif // I0STDLIB_H
