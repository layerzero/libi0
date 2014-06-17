#ifndef I0MALLOC_H
#define I0MALLOC_H

#include "stddef.h"
#include "stdio.h"

// whether the shalloc is made standalone
// if defined, each shalloc'ed range will be standalone
#define STANDALONE_SHALLOC

//remove following comment to get some debug output.
// #define _DEBUG_MLC_

//flag that whether shalloc area has been used. 
//It is in auto init area. Its init value is 0.
#define PR_IS_USED (0x200100000)

//---------------------Shalloc Defination----------------------------
//any requested size to scheduler should be aligned to 64M
#define S_ALIGN_TO_SIZE (0x4000000)

// 1 page
#define S_ALIGN_TO_SIZE_EXT (4096)

//the base address of shalloc area range array
//is a const value
#define PR_SAR_ARRAY_BASE (0x300003008)

//the end address of shalloc area range array
//is a const value
//0x10000 elements in sar_array
#define PR_SAR_ARRAY_END (0x300183008)

//the last element in shalloc area range array
//is a variable
#define PR_SAR_ARRAY_LAST_ELEMENT (0x300003000)

//malloc system call id
// defined in stddef.h
// #define SYSCALL_ID_MALLOC (1)
//---------------------End of Shalloc Defination---------------------

//---------------------Pralloc Defination----------------------------
//any requested size should be aligned to page size, namely 4K bytes.
#define P_ALIGN_TO_PAGE (0x1000)

//the base address of shalloc area range array
//is a const value
#define PR_PA_BASE (0x3FFFF0000)

//the end address of shalloc area range array
//is a const value
//0x10000 elements in sar_array
#define PR_PA_END (0x300200000)

//the last element in shalloc area range array
//is a variable
#define PR_PA_TP (0x3FFFFFFF8)
//---------------------End of Pralloc Defination---------------------


#define shalloc_option_t  uint64_t
#define sizeof_shalloc_option_t sizeof_uint64_t

#define SHALLOC_1WAY 1
#define SHALLOC_3WAY 3

void _output_debug(long x);

void init_PR_var();

char *_syscall_malloc_ext(long size, shalloc_option_t option);

char *_syscall_malloc(long size);

long s_align_to(long size);

long s_align_to_ext(long size);

long s_align_to_page(long size);

char *_get_new_sar(long size, long align_size);

char *_get_new_sar_ext(long size, long align_size, shalloc_option_t option);

char *_get_from_sar_array(long size);

void* shalloc(long size);

long p_align_to(long size);

void *_get_new_pa(long size);

void *pralloc(long size);

addr_t shalloc_ext(size_t size, shalloc_option_t option);

addr_t shalloced_replicas(addr_t addr, size_t nth);

#endif //I0MALLOC_H
