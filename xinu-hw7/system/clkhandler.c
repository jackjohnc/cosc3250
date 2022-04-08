/**
 * @file     clkhandler.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

int resched(void);

/**
 * @ingroup timer
 *
 * Interrupt handler function for the timer interrupt.  This schedules a new
 * timer interrupt to occur at some point in the future, then updates ::clktime
 * and ::clkticks, then wakes sleeping threads if there are any, otherwise
 * reschedules the processor.
 */
interrupt clkhandler(void)
{
    clkupdate(platform.clkfreq / CLKTICKS_PER_SEC);

    /* Another clock tick passes. */
    clkticks++;

    /* Update global second counter. */
    if (CLKTICKS_PER_SEC == clkticks)
    {
        clktime++;
        clkticks = 0;
    }

#if PREEMPT
    /* check to see if this proc should be preempted. */
    if (--preempt <= 0)
    {
        resched();
    }
#endif
}
