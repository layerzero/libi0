#ifndef UNISTD_H
#define UNISTD_H

#include "stdint.h"
#include "stddef.h"

// The type useconds_t is an unsigned integer type
#define useconds_t uint64_t

// sleep for us useconds
void usleep(useconds_t us);

#endif // UNISTD_H
