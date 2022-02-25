/**
 * COSC 3250 Project 5
 * Handles SWI exceptions
 * @author [Teague McGinn]
 * @author [Jack Condit]
 * Instructor [Dr. Dennis Brylow]
 * TA-BOT:MAILTO [teague.mcginn@marquette.edu]
 * TA-BOT:MAILTO [john.condit@marquette.edu]
 */

/**
 * @file xtrap.c 
 * @provides xtrap
 *
 */
/* Embedded XINU, Copyright (C) 2007, 2020.  All rights reserved. */

#include <xinu.h>

char *trap_names[] = {
    "Reset",
    "Undefined Instruction",
    "Software Interrupt (SWI)",
    "Prefetch Abort",
    "Data Abort",
    "IRQ Interrupt Request",
    "FIQ Interrupt Request"
};

/**
 * Generic XINU Trap (Interrupt) Handler.
 *
 * @param cause the contents of the cause register used to decode the trap
 * @param frame pointer to the interrupt frame with saved status
 *
 */
void xtrap(long *frame, int cause)
{
    /* Decode and handle IRQ separately. */
    if (cause == ARM_EXCEPTION_IRQ)
    {
        dispatch();
        return;
    }

    /* TODO: Implement system calls for Xinu.
     * 1) Recognize when cause of hardware exception is SWI opcode,
     * 2) Find the responsible SWI opcode in memory,
     * 3) Decode what system call was requested by examining opcode,
     * 4) Call syscall_dispatch() with the syscall number and any
     *    passed arguments, and
     * 5) On return from syscall_dispatch(), setup proper return to
     *    instruction after SWI call, with return value in place.
     */

	if (cause == ARM_EXCEPTION_SWI) {
		ulong swi = *(ulong *)frame[14]; //finds swi in frame's irq lr
		frame[14] += 4; //sets up proper return to instruction
		frame[0] = syscall_dispatch((0xFFFFFF & swi), frame);
		//bitwise & is used here to put swi in the format of an opcode for syscall_dispatch
		return;
	}

    /* If not an IRQ or SWI, fall through to generic exception handler */
    kprintf("\r\n\r\nXINU Exception [%s]\r\n", trap_names[cause]);

    kprintf("Faulting address: 0x%08X\r\n", frame[14]);
    kprintf("Faulting SPSR:    0x%08X\r\n", frame[15]);

    kprintf("[0x%08X] r12:0x%08X   lr:0x%08X\r\n",
            frame + CTX_IP, frame[CTX_IP], frame[13]);
    kprintf("[0x%08X]  r8:0x%08X   r9:0x%08X  r10:0x%08X  r11:0x%08X\r\n",
            frame + CTX_R8,
            frame[CTX_R8], frame[CTX_R9], frame[CTX_R10], frame[CTX_R11]);
    kprintf("[0x%08X]  r4:0x%08X   r5:0x%08X   r6:0x%08X   r7:0x%08X\r\n",
            frame + CTX_R4,
            frame[CTX_R4], frame[CTX_R5], frame[CTX_R6], frame[CTX_R7]);
    kprintf("[0x%08X]  r0:0x%08X   r1:0x%08X   r2:0x%08X   r3:0x%08X\r\n",
            frame + CTX_R0,
            frame[CTX_R0], frame[CTX_R1], frame[CTX_R2], frame[CTX_R3]);
    while (1)
        ;                       /* forever */
}
