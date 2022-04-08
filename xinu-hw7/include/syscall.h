/**
 * @file syscall.h
 * System call interface and handlers for user to kernel switches.
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

exchandler syscall_entry(void);
int syscall_dispatch(int, int *);

struct syscall_info
{
    int nargs;                     /**< Number of arguments required    */
    syscall (*handler) (int *);    /**< Function handler                */
};

#define SYSCALL_NONE        0 /**< Perform no operation             */
#define SYSCALL_YIELD       1 /**< Yield processor                  */
#define SYSCALL_SLEEP       2 /**< Sleep for number of microseconds */
#define SYSCALL_KILL        3 /**< Kill a process                   */
#define SYSCALL_OPEN        4 /**< Open a device                    */
#define SYSCALL_CLOSE       5 /**< Close a device                   */
#define SYSCALL_READ        6 /**< Read from a device               */
#define SYSCALL_WRITE       7 /**< Write to a device                */
#define SYSCALL_GETC        8 /**< Get character from device        */
#define SYSCALL_PUTC        9 /**< Put character to device          */
#define SYSCALL_SEEK       10 /**< Seek to position on device       */
#define SYSCALL_CONTROL    11 /**< Control a device                 */
#define SYSCALL_GETDEV     12 /**< Get a device number from string  */
#define SYSCALL_MBOXALLOC  13 /**< Allocate a mailbox               */
#define SYSCALL_MBOXFREE   14 /**< Free a mailbox                   */
#define SYSCALL_MBOXRECV   15 /**< Receive message from mailbox     */
#define SYSCALL_MBOXSEND   16 /**< Send message to mailbox          */

extern const struct syscall_info syscall_table[];
extern int nsyscalls;

/* Prototypes from user mode system calls */
syscall user_none(void);
syscall user_yield(void);
syscall user_getc(int descrp);
syscall user_putc(int descrp, char character);

#endif                          /* __SYSCALL_H__ */
