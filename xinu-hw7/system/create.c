/**
 * COSC 3250 -Project #4
 * function to create new processes in embedded O/S
 * @author [Teague McGinn]
 * @author [Jack Condit]
 * Instructor [Dr. Dennis Brylow]
 * TA-BOT:MAILTO [teague.mcginn@marquette.edu]
 * TA-BOT:MAILTO [john.condit@marquette.edu]
 */

/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, uint tickets, char *name, ulong nargs, ...)
{
    ulong *saddr;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */

    if (ssize < MINSTK)
        ssize = MINSTK;
    ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;
    /* round up to even boundary    */
    saddr = (ulong *)getmem(ssize);     /* allocate new stack and pid   */
    pid = newpid();
    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }

    numproc++;
    ppcb = &proctab[pid];
	
	// TODO: Setup PCB entry for new process.
	ppcb -> stkptr = NULL;
	ppcb->state = PRSUSP; 					//starts process in suspended
	strncpy(ppcb->name, name, PNMLEN); 			//saves name of process
	ppcb->stkbase = saddr; 					//initialzes stack base
	saddr = (ulong *)(((ulong)saddr) + ssize - sizeof(int));
	ppcb->stklen = ssize; 					//initializes the length of the stack
	ppcb->tickets = tickets;				//initialize number of tickets for process
    
    /* Initialize stack with accounting block. */
    *saddr = STACKMAGIC;
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    /* Handle variable number of arguments passed to starting function   */
    if (nargs)
    {
        pads = ((nargs - 1) / 4) * 4;
    }
    /* If more than 4 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }

	// TODO: Initialize process context.
	for (i = 0; i < 16; i++) //opens space on stack for r0-r15 
		*--saddr = 0;
	saddr[CTX_SP] = (ARM_MODE_SYS | ARM_F_BIT);
	saddr[CTX_LR] = (int) userret; //saves userret function to link register
	saddr[CTX_PC] = (int) funcaddr; //saves funcaddr to program counter
	ppcb->stkptr = saddr; //points stack pointer to the bottom of the stack

	// TODO:  Place arguments into activation record.
	//        See K&R 7.3 for example using va_start, va_arg and
	//        va_end macros for variable argument functions.
	va_start(ap, nargs);
	for (i = 0; i < nargs; i++) {
		if (i < 4)
			saddr[i] = va_arg(ap, ulong); //first 4 args go to r0-r3
		else 
			saddr[CTX_PC + (i - 3)] = va_arg(ap, ulong); //additional args go to activation record
	}
	va_end(ap);

    return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++)
    {                           /* check all NPROC slots    */
        nextpid = (nextpid + 1) % NPROC;
        if (PRFREE == proctab[nextpid].state)
        {
            return nextpid;
        }
    }
    return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
    kill(currpid);
}
