#ifndef I0MALLOC_H
#define I0MALLOC_H

#include "stddef.h"
#include "stdio.h"

//flag that whether shalloc area has been used.
//It is in auto init area. Its init value is 0.
#define _IS_USED (0x200100000)

//---------------------Shalloc Defination----------------------------
// whether the shalloc is made standalone
// if defined, each shalloc'ed range will be standalone
#define STANDALONE_SHALLOC

//the base address of shalloc area range array
#define _SAR_ARRAY_BASE (0x300003008)
//the number of elements in shalloc area range array
#define _SAR_ARRAY_SIZE (0x10000)
//the end address of shalloc area range array
#define _SAR_ARRAY_END (_SAR_ARRAY_BASE + _SAR_ARRAY_SIZE * (sizeof_uint64_t * 2 + sizeof_long * 2))

//the last element in shalloc area range array
//it is a variable
#define _SAR_ARRAY_LAST_ELEMENT (0x300003000)

//any requested size to scheduler should be aligned to a large area, like 64M
#define _S_ALIGN_TO_SIZE (0x4000000)

//shalloc options
#define shalloc_option_t long
#define sizeof_shalloc_option_t sizeof_long
#define SR_N (0)
#define AMR_N (1)
#define AMR_P_NONREP (2)
#define AMR_P_REP (3)
#define PMEM_N_REP (4)
#define PMEM_N_NONREP (5)
//---------------------End of Shalloc Defination---------------------

//---------------------Pralloc Defination----------------------------
//pralloc area range array: [_PA_END, _PA_BASE)
//the base address of pralloc area range array
#define _PA_BASE (0x3FFFF0000)
//the number of elements in pralloc area range array
#define _PA_SIZE (0xFFDF0000)
//the end address of pralloc area range array
#define _PA_END (_PA_BASE - _PA_SIZE)

//the last element in pralloc area range array
//is a variable
#define _PA_TP (0x3FFFFFFF8)

//any requested size can be aligned to a size.
#define _P_ALIGN_TO_SIZE (0x1)
//---------------------End of Pralloc Defination---------------------

void *shalloc_ext(size_t size, shalloc_option_t option);

void* shalloc(long size);

void *pralloc(long size);

#endif //I0MALLOC_H
