/**
 * COSC 3250 - Project #4
 * Building the context switch in ARM
 * @author [Teague McGinn]
 * @author [Jack Condit]
 * Instructor [Dr. Dennis Brylow]
 * TA-BOT:MAILTO [teague.mcginn@marquette.edu]
 * TA-BOT:MAILTO [john.condit@marquette.edu]
 */

/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */

#include <arm.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldstack, &newstack)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
ctxsw:
// WHAT TO BUILD:
// store r0-r11 separately in their own spots in oldStack
// store LR twice, once in stack at the r14 space and again in the PC spot
// store sp in r0 of oldStack and load sp in r1 w/o offset from newStack
// load r0-r11, LR, and PC from newStack

	//store r0-r11 separately in their own spots in the old stack
	str r0, [sp, #CTX_R0 * 4]
	str r1, [sp, #CTX_R1 * 4]
	str r2, [sp, #CTX_R2 * 4]
	str r3, [sp, #CTX_R3 * 4]
	str r4, [sp, #CTX_R4 * 4]
	str r5, [sp, #CTX_R5 * 4]
	str r6, [sp, #CTX_R6 * 4]
	str r7, [sp, #CTX_R7 * 4]
	str r8, [sp, #CTX_R8 * 4]
	str r9, [sp, #CTX_R9 * 4]
	str r10, [sp, #CTX_R10 * 4]
	str r11, [sp, #CTX_R11 * 4]
	//the link register gets saved into both the lr and pc spots
	str r14, [sp, #CTX_LR * 4]
	str r14, [sp, #CTX_PC * 4]
	//stores old stack and loads new stack
	str sp, [r0]
	ldr sp, [r1]
	//loads from new stack into r0-r11, lr, and pc
	ldr r0, [sp, #CTX_R0 * 4]
	ldr r1, [sp, #CTX_R1 * 4]
	ldr r2, [sp, #CTX_R2 * 4]
	ldr r3, [sp, #CTX_R3 * 4]
	ldr r4, [sp, #CTX_R4 * 4]
	ldr r5, [sp, #CTX_R5 * 4]
	ldr r6, [sp, #CTX_R6 * 4]
	ldr r7, [sp, #CTX_R7 * 4]
	ldr r8, [sp, #CTX_R8 * 4]
	ldr r9, [sp, #CTX_R9 * 4]
	ldr r10, [sp, #CTX_R10 * 4]
	ldr r11, [sp, #CTX_R11 * 4]
	
	ldr r14, [sp, #CTX_LR * 4]
	ldr r15, [sp, #CTX_PC * 4]

// Note: This bx lr line immediately returns from whence we came.  This
//	is here as a placeholder so that you don't crash your O/S when
//	you call ctxsw() before it is complete.  But, this line of code
//	is not actually a part of a completed solution.
//	bx	lr

	.end ctxsw
