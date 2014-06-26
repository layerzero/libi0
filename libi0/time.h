#ifndef TIME_H
#define TIME_H

#define no_DEBUG_TIME_H_

#include "stdint.h"
#include "stddef.h"

#include "stdio.h"

#define time_t long
#define sizeof_time_t sizeof_long

/* gettime sets the amount time (in tv_sec seconds and tv_nsec
 * nanoseconds) since an unspecified point in the past (for example,
 * system start-up time, or the Epoch) on the VPC a task running on.
 *
 * Note: different VPCs possibly have different start point.
 */
long gettime(time_t *tv_sec, long *tv_nsec);

/*
 * substract time tv_sec, tv_nsec from  @tv_sec_from, @tv_nsec_from
 */
void time_subtract(time_t *tv_sec_from, long *tv_nsec_from,
        time_t tv_sec, long tv_nsec);
/*
 * add time tv_sec, tv_nsec to  @tv_sec_to, @tv_nsec_to
 */
void time_add(time_t *tv_sec_to, long *tv_nsec_to,
        time_t tv_sec, long tv_nsec);


/*
 * get current time; calculate the time difference; and update the
 * time by @tv_sec and @tv_nsec.
 *
 * return 0 on success. -1 on error.
 */
long print_time_diff_and_update(time_t *base_sec, long *base_nsec);

/*
 * get current time; calculate the time difference; and update the
 * time by @tv_sec and @tv_nsec with the difference.
 *
 * return 0 on success. -1 on error.
 */
long get_time_diff_and_update(time_t *base_sec, long *base_nsec);

#endif // TIME_H
