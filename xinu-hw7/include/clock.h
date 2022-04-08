/**
 * @file clock.h
 * Definitions relating to the hardware clock and the Xinu clock interrupt.
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stddef.h>

/**
 * @ingroup timer
 *
 * Timer resolution.  This refers to the maximum number of times that Xinu
 * schedules the timer interrupt per second, @a not to the number of native
 * clock cycles per second.
 */
#define CLKTICKS_PER_SEC  1000

/**
 * These are your "quanta's". 
 *
 * The "preempt" quantum indicates when the scheduler should preempt.
 * For example, if the QUANTUM is 3, then after 3 clock ticks, the scheduler will preempt
 * and then reset the value of the preempt quantum to QUANTUM.
 */
extern volatile ulong preempt;

#define QUANTUM 3               /* clock ticks until preemption */

/**
 * clkticks and clktime keep track of information about the clock.
 */
extern volatile ulong clkticks;
extern volatile ulong clktime;

/* Clock function prototypes.  Note:  clkupdate() and clkcount() are documented
 * here because their implementations are platform-dependent.  */

void clkinit(void);

/**
 * @ingroup timer
 *
 * Sets up a timer interrupt to trigger after a certain number of clock cycles
 * have elapsed.
 *
 * @param cycles
 *     Number of cycles after which the timer interrupt is to be triggered.
 *     This refers to native clock cycles (whose frequency is specified in
 *     platform::clkfreq).
 */
void clkupdate(ulong cycles);

/**
 * @ingroup timer
 *
 * Gets the current system timer cycles.
 *
 * @return
 *	The current number of timer cycles.  Generally, only the difference between
 *	the value returned by between two successive calls to this function is
 *	meaningful.  The number of cycles that elapse per second is specified by the
 *	@ref platform::clkfreq "value in platform".
 */
ulong clkcount(void);

interrupt clkhandler(void);
void udelay(ulong);

#endif                          /* _CLOCK_H_ */
