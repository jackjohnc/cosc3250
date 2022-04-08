/**
 * @file     main.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2022.  All rights reserved. */

#include <xinu.h>

#define MAX 128

extern void testcases(void);

/**
 * Main process.  You can modify this routine to customize what Embedded Xinu
 * does when it starts up.
 */
process main(void)
{
	kprintf("Hello Xinu World!\n");	

	testcases();

    while (1)
        ;

    return 0;
}
