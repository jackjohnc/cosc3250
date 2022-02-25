/**
 * @file platform.h
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <stddef.h>
#include <stdarg.h>

/** Maximum length of platform name and family strings, including the
 * null-terminator.  */
#define PLT_STRMAX 18

/**
 * Various platform-specific parameters filled in at boot time by
 * platforminit().  When platforminit() is called this structure will initially
 * be all 0's.
 */
struct platform
{
    /** Platform descriptors as null-terminated strings.  It is
     * platform-specific what is considered a "type" and what is considered a
     * "family".  */
    char manufacturer[PLT_STRMAX];
    char family[PLT_STRMAX];
    char type[PLT_STRMAX];
    int revision;

    /** Ethernet MAC if known to boot system. */
    char macaddr[PLT_STRMAX];

    /** Minimum physical address available to the CPU.  platforminit() can leave
     * this value alone if available physical memory starts at 0.  */
    void *minaddr;

    /** One byte past the last byte physically addressable by the CPU.  Must be
     * set by platforminit().  */
    void *maxaddr;

    /** Frequency of the system timer in cycles per second.  Must be set by
     * platforminit() if RTCLOCK is enabled.  This is the frequency at which the
     * values returned by clkcount() change.  */
    ulong clkfreq;

    /** UART Clock frequency.  platforminit() only needs to set this if
     * required by the UART driver.  */
    ulong uart_clock;

    /** Number of the UART IRQ line.  platforminit() only needs to set this if
     * required by the UART driver.  */
    uchar uart_irqnum;

    /** Low word of the board's serial number.  Setting this is optional; it's
     * currently only used by the SMSC LAN9512 driver.  */
    uint serial_low;

    /** High word of the board's serial number.  Setting this is optional; it's
     * currently only used by the SMSC LAN9512 driver.  */
    uint serial_high;
};

extern struct platform platform;

int platforminit(void);

#endif                          /* _PLATFORM_H_ */
