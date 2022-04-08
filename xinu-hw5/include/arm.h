#ifndef  _ARM_H_
#define  _ARM_H_

/* Definitions of mode bits in the ARM program status register.  See: A2.2
 * "Processor Modes" of the ARM Architecture Reference Manual; also A2.5.7 "The
 * mode bits".  */
#define ARM_MODE_USR 0x10       /* Normal User Mode                                       */
#define ARM_MODE_FIQ 0x11       /* FIQ Processing Fast Interrupts Mode                    */
#define ARM_MODE_IRQ 0x12       /* IRQ Processing Standard Interrupts Mode                */
#define ARM_MODE_SVC 0x13       /* Supervisor Processing Software Interrupts Mode         */
#define ARM_MODE_ABT 0x17       /* Abort Processing memory Faults Mode                    */
#define ARM_MODE_UND 0x1B       /* Undefined Processing Undefined Instructions Mode       */
#define ARM_MODE_SYS 0x1F       /* System Running Priviledged Operating System Tasks Mode */

/* Definitions of interrupt disable bits in the ARM program status register.
 * See: A2.5.6 "The interrupt disable bits" of the ARM Architecture Reference
 * Manual.  */
#define ARM_I_BIT 0x80          /* IRQs disabled when set to 1. */
#define ARM_F_BIT 0x40          /* FIQs disabled when set to 1. */


#define ARM_EXCEPTION_RESET	0
#define ARM_EXCEPTION_UNDEFINED	1
#define ARM_EXCEPTION_SWI	2
#define ARM_EXCEPTION_PREFETCH	3
#define ARM_EXCEPTION_ABORT	4
#define ARM_EXCEPTION_IRQ	5
#define ARM_EXCEPTION_FIQ	6


#define CONTEXT 64             /**< context record size in bytes         */
                               /**< context record size in words         */
#define CONTEXT_WORDS (CONTEXT / sizeof(long))


#define CTX_R0     0            /* Argument/return/scratch registers       */
#define CTX_R1     1
#define CTX_R2     2            /* Argument/scratch registers              */
#define CTX_R3     3
#define CTX_R4     4            /* Callee-save regs                        */
#define CTX_R5     5
#define CTX_R6     6
#define CTX_R7     7
#define CTX_R8     8
#define CTX_R9     9
#define CTX_R10    10
#define CTX_R11    11
#define CTX_IP     12           /* Intra-procedural scratch register       */
#define CTX_SP     13           /* Stack pointer                           */
#define CTX_LR     14           /* Link register                           */
#define CTX_PC     15           /* Program counter                         */

#endif                          /* _ARM_H_ */
