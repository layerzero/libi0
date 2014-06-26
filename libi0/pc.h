#ifndef PC_H
#define PC_H

#include "stdint.h"
#include "stddef.h"

// fill cache of phase phase_no to from range[addr, addr+len) to range [dst, dst+len); loaded from local NRC
int64_t pc_get(int64_t phase_no, void *addr, int64_t len, void *dst);

// write local phase cache of phase phase_no from range [addr, addr+len) to local NRC
int64_t pc_write(int64_t phase_no, void *addr, int64_t len);

// synchronize phasing cache of phase phase_no
// after pc_sync, all NRCs have the whole phasing cache of phase_no
bool pc_sync(int64_t phase_no);

// pc_delete -- to be added later

#endif // PC_H
