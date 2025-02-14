/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>
#define SCARG(type, args) (type)(*args++)
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
void *sc_getmem(int *args)
{
    register memblk *prev, *curr, *leftover;

	ulong nbytes = SCARG(ulong, args);
	irqmask im;
    	if (0 == nbytes)
    	{
        	return (void *)SYSERR;
	}

    /* round to multiple of memblock size   */
    	nbytes = (ulong)roundmb(nbytes);

	im = disable();

    /* TODO:
     *      - Disable interrupts
     *      - Traverse through the freelist
     *        to find a block that's suitable
     *        (Use First Fit with remainder splitting)
     *      - Restore interrupts
     *      - return memory address if successful
     */

	curr = (struct memblock *)freelist.head;
	prev = (struct memblock *)&freelist;

	while (curr != NULL) //traverse through freelist
	{
		if ((curr->length) == nbytes)
		{
			freelist.size -= nbytes; //updating the length of the freelist
			prev->next = curr->next;
			restore(im);
			return (void*) curr;
		}
		else if ((curr->length) > nbytes)
		{
			leftover = curr + (nbytes / sizeof(struct memblock )); //splitting freelist into 2 parts
			leftover->next = curr->next;
			leftover->length = curr->length - nbytes;
			prev->next = leftover;
			freelist.size -= nbytes;
			restore(im);
			return (void*) curr;
		}
		prev = curr;
		curr = curr->next; //traversing through the list
	}
	restore(im);
	return (void *)SYSERR;
}
