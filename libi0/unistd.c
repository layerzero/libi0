#include "unistd.h"

// sleep for us useconds
void usleep(useconds_t us)
{
    // set system call id
    *(SYSCALL_ID_TYPE*) SYSCALL_ID_ADDR = (SYSCALL_ID_TYPE) SYSCALL_ID_USLEEP;

    // set system call input arguments
    *(useconds_t*) SYSCALL_COMM_AREA_ADDR = us;

    // issue system call
    asm("int 0x80");

    return;
}

#endif // UNISTD_H
