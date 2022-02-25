/**
 * @file bcm2837.h
 *
 * Definitions specific to the BCM2837 SoC used in the Raspberry Pi 3.
 *
 * Note that although some of the numbers defined in this file are documented in
 * Broadcom's "BCM2837 ARM Peripherals" document, unfortunately some could only
 * be found in the Linux source (arch/arm/mach-bcm2708/include/mach/platform.h).
 */

#ifndef _BCM2837_H_
#define _BCM2837_H_

#include <stddef.h>

/********************************************************************
 * ARM physical memory addresses of selected BCM2837 peripherals    *
 ********************************************************************/

/* Start of memory-mapped peripherals address space  */
#define PERIPHERALS_BASE 0x3F000000UL

/* System timer  */
#define SYSTEM_TIMER_REGS_BASE (PERIPHERALS_BASE + 0x3000)

/* Interrupt controller (for ARM)  */
#define INTERRUPT_REGS_BASE    (PERIPHERALS_BASE + 0xB200)

/* Mailbox  */
#define MAILBOX_REGS_BASE      (PERIPHERALS_BASE + 0xB880)

/* Power management / watchdog timer  */
#define PM_REGS_BASE           (PERIPHERALS_BASE + 0x100000)

/* PL011 UART  */
#define PL011_REGS_BASE        (PERIPHERALS_BASE + 0x201000)

/* GPIO */
#define GPIO_REGS_BASE         (PERIPHERALS_BASE + 0x200000)

/* SD host controller  */
#define SDHCI_REGS_BASE        (PERIPHERALS_BASE + 0x300000)

/* Synopsys DesignWare Hi-Speed USB 2.0 On-The-Go Controller  */
#define DWC_REGS_BASE          (PERIPHERALS_BASE + 0x980000)

/* Start of local peripherals address space */
#define LOCAL_PERIPHERALS_BASE 0x40000000UL

/* Timers interrupt control registers */
#define CORE0_TIMER_IRQCNTL	(LOCAL_PERIPHERALS_BASE + 0x0040)
#define CORE1_TIMER_IRQCNTL	(LOCAL_PERIPHERALS_BASE + 0x0044)
#define CORE2_TIMER_IRQCNTL	(LOCAL_PERIPHERALS_BASE + 0x0048)
#define CORE3_TIMER_IRQCNTL	(LOCAL_PERIPHERALS_BASE + 0x004C)

/* Where to route timer interrupt to, IRQ/FIQ    */
/* Setting both the IRQ and FIQ bit gives an FIQ */
#define TIMER0_IRQ 0x01
#define TIMER1_IRQ 0x02
#define TIMER2_IRQ 0x04
#define TIMER3_IRQ 0x08
#define TIMER0_FIQ 0x10
#define TIMER1_FIQ 0x20
#define TIMER2_FIQ 0x40
#define TIMER3_FIQ 0x80

/* Mailbox interrupt control registers */
#define CORE0_MBOX_IRQCNTL	(LOCAL_PERIPHERALS_BASE + 0x0050)
#define CORE1_MBOX_IRQCNTL	(LOCAL_PERIPHERALS_BASE + 0x0054)
#define CORE2_MBOX_IRQCNTL	(LOCAL_PERIPHERALS_BASE + 0x0058)
#define CORE3_MBOX_IRQCNTL	(LOCAL_PERIPHERALS_BASE + 0x005C)

/* Where to route mailbox interrupt to, IRQ/FIQ  */
/* Setting both the IRQ and FIQ bit gives an FIQ */
#define MBOX0_IRQ 0x01
#define MBOX1_IRQ 0x02
#define MBOX2_IRQ 0x04
#define MBOX3_IRQ 0x08
#define MBOX0_FIQ 0x10
#define MBOX1_FIQ 0x20
#define MBOX2_FIQ 0x40
#define MBOX3_FIQ 0x80

/* IRQ & FIQ source registers */
#define CORE0_IRQ_SOURCE 	(LOCAL_PERIPHERALS_BASE + 0x0060)
#define CORE1_IRQ_SOURCE 	(LOCAL_PERIPHERALS_BASE + 0x0064)
#define CORE2_IRQ_SOURCE 	(LOCAL_PERIPHERALS_BASE + 0x0068)
#define CORE3_IRQ_SOURCE 	(LOCAL_PERIPHERALS_BASE + 0x006C)
#define CORE0_FIQ_SOURCE 	(LOCAL_PERIPHERALS_BASE + 0x0070)
#define CORE1_FIQ_SOURCE 	(LOCAL_PERIPHERALS_BASE + 0x0074)
#define CORE2_FIQ_SOURCE 	(LOCAL_PERIPHERALS_BASE + 0x0078)
#define CORE3_FIQ_SOURCE 	(LOCAL_PERIPHERALS_BASE + 0x007C)

/* Interrupt source bits                  */
/* IRQ and FIQ are the same               */
/* GPU bits can be set for one core only  */
#define INT_SRC_TIMER0 0x00000001
#define INT_SRC_TIMER1 0x00000002
#define INT_SRC_TIMER2 0x00000004
#define INT_SRC_TIMER3 0x00000008
#define INT_SRC_MBOX0 0x00000010
#define INT_SRC_MBOX1 0x00000020
#define INT_SRC_MBOX2 0x00000040
#define INT_SRC_MBOX3 0x00000080
#define INT_SRC_GPU 0x00000100
#define INT_SRC_PMU 0x00000200

/***************************************************************************
 * IRQ lines of selected BCM2837 peripherals.  Note about the numbering    *
 * used here:  IRQs 0-63 are those shared between the GPU and CPU, whereas *
 * IRQs 64+ are CPU-specific.                                              *
 ***************************************************************************/

/* System timer - one IRQ line per output compare register.  */
#define IRQ_SYSTEM_TIMER_0 0
#define IRQ_SYSTEM_TIMER_1 1
#define IRQ_SYSTEM_TIMER_2 2
#define IRQ_SYSTEM_TIMER_3 3

/* Timer IRQ to use by default.  Note: this only be either 1 or 3, since 0 and 2
 * are already used by the VideoCore.  */
#define IRQ_TIMER          IRQ_SYSTEM_TIMER_3

/* Synopsys DesignWare Hi-Speed USB 2.0 On-The-Go Controller  */
#define IRQ_USB            9

/* PCM sound  */
#define IRQ_PCM            55

/* PL011 UART  */
#define IRQ_PL011          57

/* SD card host controller  */
#define IRQ_SD             62

/************************************************************************
 * Interfaces to memory barriers for peripheral access.                 *
 *                                                                      *
 * These are necessary due to the memory ordering caveats documented in *
 * section 1.3 of Broadcom's "BCM2837 ARM Peripherals" document.        *
 ************************************************************************/

extern void dmb(void);

#endif                          /* _BCM2837_H_ */
