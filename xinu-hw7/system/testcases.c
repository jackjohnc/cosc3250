/**
 * @file testcases.c
 * @provides testcases
 *
 * TA-BOT:MAILTO
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

extern void main(int, char *);

int testmain(int argc, char **argv)
{
    int i;
    kprintf("Hello XINU World!\r\n");

    for (i = 0; i <= 10; i++)
        kprintf("This is process %d\r\n", currpid);
    return 0;
}

int testwhile(int argc, char **argv) {
	kprintf("Hello XINU World!\r\n");
	while(1) kprintf("This is process %d\r\n", currpid);
	return 0;
}

void testbigargs(int a, int b, int c, int d, int e, int f, int g, int h)
{
    kprintf("Testing bigargs...\r\n");
    kprintf("a = 0x%08X\r\n", a);
    kprintf("b = 0x%08X\r\n", b);
    kprintf("c = 0x%08X\r\n", c);
    kprintf("d = 0x%08X\r\n", d);
    kprintf("e = 0x%08X\r\n", e);
    kprintf("f = 0x%08X\r\n", f);
    kprintf("g = 0x%08X\r\n", g);
    kprintf("h = 0x%08X\r\n", h);
}

void printFreeList(void) {
	memblk *p = freelist.head;
	while (p != NULL) {
		kprintf("[0x%08X : %10d : 0x%08X]\r\n", p, p->length, p->next);
		p = p->next;
	}
}

void printpcb(int pid)
{
    pcb *ppcb = NULL;

    /* Using the process ID, access it in the PCB table. */
    ppcb = &proctab[pid];

    /* Printing PCB */
    kprintf("Process name		  : %s \r\n", ppcb->name);

    switch (ppcb->state)
    {
    case PRFREE:
        kprintf("State of the process	  : FREE \r\n");
        break;
    case PRCURR:
        kprintf("State of the process 	  : CURRENT \r\n");
        break;
    case PRSUSP:
        kprintf("State of the process	  : SUSPENDED \r\n");
        break;
    case PRREADY:
        kprintf("State of the process	  : READY \r\n");
        break;
    default:
        kprintf("ERROR: Process state not correctly set!\r\n");
        break;
    }

    /* Print PCB contents and registers */
    kprintf("Base of run time stack    : 0x%08X \r\n", ppcb->stkbase);
    kprintf("Stack length of process   : %8u \r\n", ppcb->stklen);
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;
    void * pmem, * pmem1, * pmem2;

    kprintf("q) getmem one time, printing the freelist before and after\r\n");
    kprintf("w) getmem once, then freemem once\r\n");
    kprintf("e) free block between two allocated blocks\r\n");
    kprintf("r) simple case using malloc and free\r\n");
    kprintf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!

    
    c = kgetc();
    switch (c)
    {
	case 'q':
		kprintf("empty free list: \r\n");
		printFreeList();
		getmem(30);
		kprintf("\r\ngetmem1: \r\n");
		printFreeList();
		getmem(30);
		kprintf("\r\ngetmem2: \r\n");
		printFreeList();
		break;

	case 'w':
		kprintf("empty free list: \r\n");
		printFreeList();
		pmem = getmem(32);
		printFreeList();
		freemem(pmem, 32);
		printFreeList();
		break;

	case 'e':
		printFreeList();
		kprintf("\r\n");
		pmem = getmem(64);
		printFreeList();
		kprintf("\r\n");
		pmem1 = getmem(128);
		printFreeList();
		kprintf("\r\n");
	        pmem2 = getmem(256);
		printFreeList();
		kprintf("\r\n");
		freemem(pmem1, 128);
		printFreeList();
		kprintf("\r\n");
		freemem(pmem2, 256);
		printFreeList();
		break;

	case 'r':
		printFreeList();
		pmem = malloc(128);
		printFreeList();
		pmem1 = malloc(256);
		printFreeList();
		free(pmem);
		printFreeList();
		break;

    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
