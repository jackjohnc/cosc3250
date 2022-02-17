/**
 * @file kprintf.c
 */

/* Embedded Xinu, Copyright (C) 2009, 2022.  All rights reserved. */

#include <xinu.h>

/**
 * Synchronously read a character from a UART.  This blocks until a character is
 * available.  The interrupt handler is not used.
 *
 * @return
 *      The character read from the UART cast to an <code>int</code>.
 *      If ASCII value 4 is seen, return special constant EOF instead.
 */
syscall kgetc(void)
{
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    // TODO: Check UART flags register, and
    //       once the receiver is not empty, get and return character c.
    //       If ASCII value 4 is seen, return special constant EOF instead.

	char receive;

    while ((regptr->fr & PL011_FR_RXFE) != 0) {}
		//in the while conditional, placing 16 in the place of the RXFE Macro would work, as it's the FIFO in the 5th bit position, meaning its value is either 0 or 16 when represented by an integer.
	receive = regptr->dr;
	if (receive == 4) return EOF;
	else return receive;

    return SYSERR;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
    volatile struct pl011_uart_csreg *regptr;
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    // TODO: Check the UART for characters, and return true or false.
    
	//char receive;
	//while ((regptr->fr & PL011_FR_RXFE) != 0) {}
	//receive = regptr->dr;
	if (regptr->fr & PL011_FR_RXFE) return 0;
	else return 1;

    return SYSERR;
}

/**
 * Synchronously write a character to a UART.  This blocks until the character
 * has been written to the hardware.  The interrupt handler is not used.
 *
 * @param c
 *      The character to write.
 *
 * @return
 *      The character written to the UART.
 */
syscall kputc(uchar c)
{
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    // TODO: Check UART flags register.
    //       Once the Transmitter FIFO is not full, send character c.
    //       Special handling -- if c is newline, transmit carriage return
    //       followed by newline.

	while ((regptr->fr & PL011_FR_TXFF) != 0) {}
	if (c == '\n') {
		regptr->dr = '\r';
		while ((regptr->fr & PL011_FR_TXFF) != 0) {}
		regptr->dr = '\n';
	}
	else regptr->dr = c;

    return SYSERR;
}

/**
 * kernel printf: formatted, synchronous output to SERIAL0.
 *
 * @param format
 *      The format string.  Not all standard format specifiers are supported by
 *      this implementation.  See _doprnt() for a description of supported
 *      conversion specifications.
 * @param ...
 *      Arguments matching those in the format string.
 *
 * @return
 *      The number of characters written.
 */
syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(int, int))kputc, 0);
    va_end(ap);
    return retval;
}
