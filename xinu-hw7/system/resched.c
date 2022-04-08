/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

extern void ctxsw(void *, void *);

int lotterySched(void);

/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
syscall resched(void)
{
	irqmask im;
	im = disable();

    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */

    oldproc = &proctab[currpid];

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid, readylist);
    }

    /**
     * We recommend you use a helper function for the following:
     * TODO: Get the total number of tickets from all processes that are in current and ready states.
     * Utilize the random() function to pick a random ticket. 
     * Traverse through the process table to identify which proccess has the random ticket value.
     * Remove process from queue.
     * Set currpid to the new process.
     */
	currpid = lotterySched(); //my scheduling process
	remove(currpid);
    //currpid = dequeue(readylist); removed
    newproc = &proctab[currpid];
    newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
    preempt = QUANTUM;
#endif

    ctxsw(&oldproc->stkptr, &newproc->stkptr);
	restore(im);
    /* The OLD process returns here when resumed. */
    return OK;
}

int lotterySched(void) {
	int i;
	int tickTotal = 0;
	int current = 0;

	//for loop finds the number of tickets for current and ready processes
	for(i = 0; i < NPROC; i++) {
		if ((proctab[i].state == PRCURR) || (proctab[i].state == PRREADY))
			tickTotal += proctab[i].tickets;
	}

	int winner = random(tickTotal);	//finds a winning ticket using random
	i = 0;

	//finds the winning process and breaks when it finds it
	//loop condition prevents infinite loops
	
	//while (current != winner)
	while (i < NPROC) {
		if ((proctab[i].state == PRCURR) || (proctab[i].state == PRREADY)) {
			current += (&proctab[i])->tickets;
			if (current > winner) break;
			i++;
		}
	}
	return i;
}
