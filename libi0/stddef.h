
#ifndef I0STDDEF_H
#define I0STDDEF_H

#include "stdint.h"

// task marker
#define __task__

// consts
#define C_0 (0)
#define C_n (10)
#define C_r (13)
#define C_CR (13)
#define C_t (9)

// registers only supports 4 yet

register long reg1;
register long reg2;
register long reg3;
register long reg4;


// system confgiruation
#define PAGE_SIZE (4096)

#define N_LONG_PER_PAGE (512)
#define N_CHAR_PER_PAGE PAGE_SIZE

// sizes
#define size_t int64_t
#define sizeof_size_t 8

#define addr_t uint64_t

// #define void char

// Note: these configurations should be consistent with the sys_config.h of
// L0

// the starting address of SR
// #define SR_BEGIN	0x400000000
#define SR_BEGIN	0x800000000
// the ending address of SR
#define SR_END		0x100000000000

// Deja vu memory
#define DEJAVU_MEM_BEGIN (0x400000000)
#define DEJAVU_MEM_SIZE  (0x400000000)

// Phasing cache
#define PHASING_CACHE_ADDR (0x200200000L)
#define PHASING_CACHE_LEN   (0xc0000000L)


// otherwise 8TB
#define PPM_12TB 1


// STDIN
#define STDIN     (0x100000200)

// STDOUT
#define STDOUT    (0x100000208)

// memory of address that "stores" runner ID
#define RUNNER_ID (0x100000120)

// NULL
#define NULL      0


#ifdef PPM_12TB



// Obsoleted: planetary memory range (12TB)
#define PPM_BEGIN  0x40000000000
#define PPM_SIZE   0xc0000000000
#define PPM_END    0xfffffffffff


// further partitioning the PPM (persistent memory)

// normal SR (in physical memory) size
// 2TB
#define SR_N_SIZE  0x20000000000

// normal SR begin and end
#define SR_N_BEGIN (SR_BEGIN)
#define SR_N_END (SR_BEGIN + SR_N_SIZE - 1)

// normal AMR (in physical memory) size
// 2TB
#define AMR_N_OFFSET_SIZE 0x20000000000

#define AMR_N_OFFSET_BEGIN (SR_N_END + 1)
#define AMR_N_OFFSET_END (AMR_N_OFFSET_BEGIN + AMR_N_OFFSET_SIZE - 1)

// persistent AMR size
// 6TB
#define AMR_P_OFFSET_SIZE 0x60000000000

// replicated AMR_P; it is close to the PMEM_N
#define AMR_P_REP_OFFSET_SIZE 0x40000000000

// replicated PMEM_N; it is close to the AMR_P
#define PMEM_N_REP_SIZE       0x40000000000

// replicated pages' range size
#define PMEM_REP_SIZE (AMR_P_REP_OFFSET_SIZE + PMEM_N_REP_SIZE)

#define AMR_P_OFFSET_BEGIN (AMR_N_OFFSET_END + 1)
#define AMR_P_OFFSET_END (AMR_P_OFFSET_BEGIN + AMR_P_OFFSET_SIZE - 1)

#define AMR_P_REP_OFFSET_END (AMR_P_OFFSET_END)
#define AMR_P_REP_OFFSET_BEGIN (AMR_P_REP_OFFSET_END + 1 - AMR_P_REP_OFFSET_SIZE)

// replicated pages' range
#define PMEM_REP_BEGIN AMR_P_REP_OFFSET_BEGIN
#define PMEM_REP_END (PMEM_REP_BEGIN + PMEM_REP_SIZE - 1)

// normal (not in AMR) persistent memory size
// 6TB
#define PMEM_N_SIZE       0x60000000000

#define PMEM_N_BEGIN (AMR_P_OFFSET_END + 1)
#define PMEM_N_END (PMEM_N_BEGIN + PMEM_N_SIZE - 1)

#define PMEM_N_REP_BEGIN (PMEM_N_BEGIN)
#define PMEM_N_REP_END (PMEM_N_REP_BEGIN + PMEM_N_REP_SIZE - 1)

// non-replicated pmem
#define PMEM_N_NONREP_BEGIN (PMEM_N_REP_END + 1)

// Note: in total persistent memory size is PPM_SIZE

// Note: libi0 and KV+ need to be changed accordingly

// AMR ranges
// Option 1: all the SR except PPM
// #define AMR_OFFSET_BEGIN SR_BEGIN
#define AMR_OFFSET_BEGIN AMR_N_OFFSET_BEGIN
// #define AMR_OFFSET_END   PPM_BEGIN
// for KV+
// #define AMR_OFFSET_END   0x54000000000
#define AMR_OFFSET_END   AMR_P_OFFSET_END


// replication factor
#define PAGE_REP_FACTOR 3

// number of replication regions
#define N_REP_REGION 3


// idle handler heap
#define IDLE_HANDLE_RUNNER_HEAP_BEGIN (0x3f800000000)
#define IDLE_HANDLE_RUNNER_HEAP_LEN   (0x1000)

#else // default 8TB

// Obsoleted: planetary memory range (8TB)
#define PPM_START  0x80000000000
#define PPM_SIZE   0x80000000000
#define PPM_END    0xfffffffffff

// idle handler heap
#define IDLE_HANDLE_RUNNER_HEAP_BEGIN (0x7f800000000)
#define IDLE_HANDLE_RUNNER_HEAP_LEN (0x1000)

#endif

// system call
#define SYSCALL_ID_ADDR (0x100000420)
#define SYSCALL_COMM_AREA_ADDR (0x100001000)
#define SYSCALL_COMM_AREA_LEN (0x1000)

#define SYSCALL_ID_TYPE uint64_t

// syscall ID
#define SYSCALL_ID_MALLOC  (1)
#define SYSCALL_ID_MALLOC_EXT  (6)

#define SYSCALL_ID_USLEEP  (2)
#define SYSCALL_ID_GETTIME (7)

// phasing cache
#define SYSCALL_ID_PC_GET    (8)
#define SYSCALL_ID_PC_WRITE  (9)
#define SYSCALL_ID_PC_DELETE (10)
#define SYSCALL_ID_PC_SYNC   (11)

// streaming I/O
#define SYSCALL_ID_SREAD   (3)
#define SYSCALL_ID_SWRITE  (4)
#define SYSCALL_ID_SLISTEN (5)

// systemEx
#define SYSCALL_ID_SYSTEMEX (12)

// space related
// suggested space specifier format
#define SPACE(I)       SPACE_START(I)

#define SPACE_START(I) (SPACE_INC*(I))

#define SPACE_INC        (0x400000000000)

// obsoleted space specifiers
#define L0_SPACE0         SPACE(0)
#define L0_SPACE1         SPACE(1)
#define L0_SPACE2         SPACE(2)
#define L0_SPACE3         SPACE(3)
#define L0_SPACE4         SPACE(4)
#define L0_SPACE5         SPACE(5)
#define L0_SPACE6         SPACE(6)
#define L0_SPACE7         SPACE(7)
#define L0_SPACE8         SPACE(8)
#define L0_SPACE9         SPACE(9)
#define L0_SPACE10        SPACE(10)
#define L0_SPACE11        SPACE(11)
#define L0_SPACE12        SPACE(12)

#define L0_SPACE_INC        SPACE_INC


// exit code
#define NEW_RUNNER_ID_ADDR (0x100000410)
#define EC_RANGE           (0x1000)

// functions

// get current task's ID
long get_my_task_id();

// get the new tasks' ID.
// should be called just after invoking "runner" or spawn/spawnx
long get_new_task_id();

// calculate the exit code range from the ID
// len in bytes
void task_id_to_ec_range(long id, long *base, long *len);

#endif // I0STDDEF_H
