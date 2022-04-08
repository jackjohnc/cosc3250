/**
 * @file memory.h
 * Definitions for kernel memory allocator and maintenance.
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#ifndef _MEMORY_H_
#define _MEMORY_H_

/* Other memory data */

extern void *_end;              /* linker provides end of image       */
extern void *memheap;           /* bottom of heap                     */

/* Memory function prototypes */
void *stkget(uint);

#endif                          /* _MEMORY_H_ */
