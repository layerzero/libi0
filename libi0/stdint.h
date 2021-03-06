#ifndef STDINT_H
#define STDINT_H

#define uint64_t long
#define int64_t long

#define sizeof_long 8
#define sizeof_double 8

#define sizeof_uint64_t sizeof_long
#define sizeof_int64_t sizeof_long

#define sizeof_char 1

// store a 2-byte into addr
void store_int16(void *addr, long n);

// load a 2-byte into addr
long load_int16(void *addr);

#endif // STDINT
