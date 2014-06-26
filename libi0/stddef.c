#include "stddef.h"

// functions

// get current task's ID
long get_my_task_id()
{
    return *(long*)(RUNNER_ID);
}

// get the new tasks' ID.
// should be called just after invoking "runner" or spawn/spawnx
long get_new_task_id()
{
    return *(long*)(NEW_RUNNER_ID_ADDR);
}

// calculate the exit code range from the ID
// len in bytes
void task_id_to_ec_range(long id, long *base, long *len)
{
    *base = id;
    *len = EC_RANGE;
    return;
}
