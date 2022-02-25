/**
 * @file queue.h 
 * @provides firstid, isempty, nonempty
 * 
 * The process queue system allows a statically-allocated array to
 * model sorted process queueing before more complex operating system
 * services, like dynamic memory allocation, are up and running.
 *
 * These process queues assume several important invariants:
 *  1) A fixed number of process queues in the system, known
 *     at compile time,
 *  2) A process queue can contain only processes and their
 *     associated key values, and thus queue length is at
 *     most NPROC, the total number of processes, and
 *  3) A process can be in at most one queue at a time.
 *
 * Ordering of processes within a given queue depends upon the sorting
 * functions called when maintaining that particular system queue.
 *
 * $Id: queue.h 189 2007-07-13 21:43:45Z brylow $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/* queue structure declarations, constants, and inline procedures       */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#ifndef NQENT
#define NQENT   NPROC   /**< one for each process                        */ \
          + 2           /**< plus two for each list (ready list)         */
#endif

typedef unsigned long qid_typ;

struct qentry
{                       /**< one for each process plus two for each list */
    pid_typ next;       /**< index of next process or tail               */
    pid_typ prev;       /**< index of previous process or head           */
};

extern struct qentry queuetab[];
extern qid_typ readylist;

/* inline list manipulation procedures                                   */

#define queuehead(q) (((q) >> 16) & 0xFFFF)
#define queuetail(q) ((q) & 0xFFFF)

#define EMPTY (-2)

/** 
 * Check for invalid process ids.  Note that interrupts must be disabled
 * for the condition to hold true between statements.
 */
#define isbadqueue(x) (queuehead(x)<0 || (queuehead(x) != (queuetail(x) - 1)) || queuetail(x)>=NQENT)

#define isempty(q)   (queuetab[queuehead(q)].next >= NPROC)
#define nonempty(q)  (queuetab[queuehead(q)].next < NPROC)
#define firstid(q)   (queuetab[queuehead(q)].next)

pid_typ enqueue(pid_typ, qid_typ);
pid_typ remove(pid_typ);
pid_typ dequeue(qid_typ);
qid_typ newqueue(void);
qid_typ prioritize(pid_typ pid, qid_typ q, ulong key);

#endif                          /* _QUEUE_H_ */
