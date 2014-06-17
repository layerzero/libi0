#ifndef PC_H
#define PC_H

#include "stdint.h"
#include "stddef.h"

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

// pc_delete -- to be added later

#endif // PC_H
