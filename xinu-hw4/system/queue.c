/**
 * @file queue.c
 * @provides enqueue, remove, dequeue, getfirst, newqueue
 *
 * $Id: queue.c 189 2007-07-13 21:43:45Z brylow $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

struct qentry queuetab[NQENT];  /**< global queue table                   */
static int nextqueue = NPROC;   /**< queuetab[0..NPROC-1] are processes   */

/**
 * Insert a process at the tail of a queue
 * @param  pid process ID to enqueue
 * @param  q   queue to which the process should be added
 * @return process id of enqueued process
 */
pid_typ enqueue(pid_typ pid, qid_typ q)
{
    int tail;

    if (isbadqueue(q) || isbadpid(pid))
    {
        return SYSERR;
    }

    tail = queuetail(q);

    queuetab[pid].next = tail;
    queuetab[pid].prev = queuetab[tail].prev;
    queuetab[queuetab[tail].prev].next = pid;
    queuetab[tail].prev = pid;
    return pid;
}


/**
 * Remove a process from anywhere in a queue
 * @param  pid process ID to remove
 * @return process id of removed process
 */
pid_typ remove(pid_typ pid)
{
    if (isbadpid(pid))
    {
        return SYSERR;
    }

    queuetab[queuetab[pid].prev].next = queuetab[pid].next;
    queuetab[queuetab[pid].next].prev = queuetab[pid].prev;
    return pid;
}

/**
 * Remove and return the first process on a list
 * @param  q      queue from which process should be removed
 * @return process id of removed process, or EMPTY
 */
pid_typ dequeue(qid_typ q)
{
    int head = queuehead(q);
    pid_typ pid;                /* first process on the list    */

    if (isbadqueue(q))
    {
        return SYSERR;
    }

    if ((pid = queuetab[head].next) < NPROC)
    {
        remove(pid);
        queuetab[pid].prev = pid;
        queuetab[pid].next = pid;
        return pid;
    }
    else
    {
        return EMPTY;
    }
}

/**
 * Initialize a new queue in the global process queue table
 * @return newly allocated 
 */
qid_typ newqueue(void)
{
    int head, tail;

    head = nextqueue++;
    tail = nextqueue++;
    queuetab[head].next = tail;
    queuetab[head].prev = EMPTY;
    queuetab[tail].next = EMPTY;
    queuetab[tail].prev = head;
    return (qid_typ) ((head << 16) | (tail & 0xFFFF));
}
