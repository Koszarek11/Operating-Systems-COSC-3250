/**
 * @file kprintf.c
 * COSC Project 3
 * @author Vicenzo Infusino and Klemens Koszarek
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO klemens.koszarek@marquette.edu vicenzo.infusino@marquette.edu
 * This kprintf.c file has 3 functions, kputc(), kgetc(), and kcheckc(). 
 * We use this file to call in our main method for codebreaker. 
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
	int c;
	while ((regptr -> fr & PL011_FR_RXFE) == PL011_FR_RXFE) // while the receiver isn't empty
	{
	}
	c = regptr -> dr; //saving data from data register into c
	if (c == 4)
	{
		return EOF; //uses enters control d
	}
	else
	{
		return c; //returns character from data register once receiver isn't empty
	}
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
	if (regptr -> fr & PL011_FR_RXFE)
	{
		return 0; // if the flag register is full, it returns false
	}
	else
	{
		return 1; // will return true if a character is available
	}
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

	while ((regptr -> fr & PL011_FR_TXFF) == PL011_FR_TXFF) //while the transmitter is full
	{
	}
	if (c == '\n') //if c is equal to newline, we transmit carriage return
	{
		regptr -> dr = '\r'; //tranmits carriage return
		while(regptr -> fr & PL011_FR_TXFF)
		{
		}
		regptr -> dr = '\n'; //changes into newline
	}
	else
	{
		regptr -> dr = c; //saving data from data register into c
	}
	return c; //returns character if transmitter FIFO isn't full

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
    irqmask im;
    im = disable();

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(int, int))kputc, 0);
    va_end(ap);

    restore(im);

    return retval;
}
