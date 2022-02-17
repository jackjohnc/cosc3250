/**
 * @file ready.c
 * @provides ready
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/**
 * Make a process eligible for CPU service.
 * @param pid process id of the process to move to the ready state
 * @param resch if TRUE, reschedule will be called
 * @return OK if the process has been added to the ready list, else SYSERR
 */
syscall ready(pid_typ pid, bool resch)
{
    register pcb *ppcb;

    ASSERT(!isbadpid(pid));

    ppcb = &proctab[pid];
    ppcb->state = PRREADY;

    enqueue(pid, readylist);

    if (resch)
    {
        resched();
    }
    return OK;
}
