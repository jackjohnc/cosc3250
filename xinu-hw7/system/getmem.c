/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Allocate heap memory.
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
void *getmem(ulong nbytes)
{
    register memblk *prev, *curr, *leftover;
    irqmask im;

    im = disable();

    if (0 == nbytes)
    {
	restore(im);
        return (void *)SYSERR;
    }
    /*
    else if (nbytes > freelist.size)
    {
	restore(im);
	return (void *)SYSERR;
    }
    */
    /* round to multiple of memblock size   */
    nbytes = (ulong)roundmb(nbytes);

    /* TODO:
     *      + Disable interrupts
     *      + Traverse through the freelist
     *        to find a block that's suitable 
     *        (Use First Fit with remainder splitting)
     *      + Restore interrupts
     *      + return memory address if successful
     */

    curr = (struct memblock *)freelist.head;
    prev = (struct memblock *)&freelist;

    while (curr != NULL) {
    	if (curr->length == nbytes) {
		freelist.size -= nbytes;
		prev->next = curr->next;
		restore(im);
		return (void *)curr;
	}
	else if ((curr->length) > nbytes) {
		leftover = curr + (nbytes / sizeof(memblk));
		leftover->next = curr->next;
		leftover->length = (curr->length) - nbytes;
		prev->next = leftover;
		freelist.size -= nbytes;
		restore(im);
		return (void *)curr;
	}
	//if curr->length < nbytes, then it will go here and move to the next spot in heap
	prev = curr;
	curr = curr->next;
    }

    restore(im);

    return (void *)SYSERR;
}
