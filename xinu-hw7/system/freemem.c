/**
 * @file freemem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with memget().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to memget().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, ulong nbytes)
{
    register struct memblock *block, *next, *prev;
    ulong top;
    irqmask im;

    im = disable();

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < freelist.base)
        || ((ulong)memptr > freelist.base + freelist.bound))
    {
    	restore(im);
        return SYSERR;
    }
    /*
    if (nbytes > freelist.size)
    {
	nbytes = freelist.size;
    }
    */
    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

    /* TODO:
     *      + Disable interrupts
     *      - Find where the memory block should
     *        go back onto the freelist (based on address)
     *      - Find top of previous memblock
     *      - Make sure block is not overlapping on prev or next blocks
     *      - Coalesce with previous block if adjacent
     *      - Coalesce with next block if adjacent
     *      - Restore interrupts
     */

    next = (struct memblock *)freelist.head;
    prev = (struct memblock *)&freelist;
    top = NULL;

    while (next != NULL) {
	    if (block < next) {
		    block->next = next;
		    block->length = nbytes;
		    if (!((ulong)prev == (ulong)&freelist))
		    	top = (ulong)prev + prev->length;
		    if (((block + ((block->length) / sizeof(memblk))) > next) || (top > (ulong)block)) {
			    restore(im);
			    return SYSERR;
		    }
		    freelist.size += nbytes;
		    if ((prev + ((prev->length) / sizeof(memblk))) == block) {
			    prev->next = block->next;
			    prev->length += block->length;
			    block = prev;
		    }
		    else {
			    prev->next = block;
		    }
		    if ((block + ((block->length) / sizeof(memblk))) == next) {
			    block->next = next->next;
			    block->length += next->length;
		    }
		    restore(im);
		    return OK;
	    }
	    prev = next;
	    next = next->next;
    }

    restore(im);

    return OK;
}
