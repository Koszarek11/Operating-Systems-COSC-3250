/**
 * @file malloc.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Request heap storage, record accounting information, returning pointer
 * to assigned memory region.
 *
 * @param size
 *      number of bytes requested
 *
 * @return
 *      pointer to region on success, or @c NULL on failure
 */
void *malloc(ulong size)
{
    struct memblock *pmem;

    /* we don't allocate 0 bytes. */
    if (0 == size)
    {
       return NULL;
    }

     /** TODO:
      *      1) Make room for accounting info
      *      2) Acquire memory with getmem syscall
      *         - handle possible error (SYSERR) from getmem...
      *      3) Set accounting info in pmem
      *      4) Return proper pointer to base of free memory region
      */

	size = size + sizeof(struct memblock); //getting room for accounting info (extra 8 spaces)
	pmem = getmem(size);
	if ((ulong)pmem == SYSERR)
	{
		return NULL;
	}
	else
	{
		pmem->length = size;
		pmem->next = pmem;
	}

    return (void *)(pmem + 1);
}
