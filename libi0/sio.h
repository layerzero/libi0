#ifndef SIO_H
#define SIO_H

#include "stdint.h"
#include "stddef.h"

#define no_SIO_DEBUG_ 1

#define sid_t uint64_t
#define sizeof_sid_t 8

// an invalid sid_t
// Note: should be consistent with the underlying syscall handler
#define SID_INVALID 0

// Read at most @nbyte from @sid to @addr.
// Returned value is:
// the number of bytes received,
// or -1 if an error occurred,
// or 0 when the peer has performed an orderly shutdown.
size_t sread(sid_t sid, char *addr, size_t nbyte);

// Write at most @nbyte from @addr to @sid.
// Returned value is:
// the number of bytes written.
size_t swrite(sid_t sid, char *addr, size_t nbyte);

// Listen on port @port
// On success, return the sid
// On failure, return SID_INVALID
sid_t slisten(size_t port);

#endif // SIO_H

