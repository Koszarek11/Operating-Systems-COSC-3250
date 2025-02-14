/**
 * @file freemem.c
 * COSC 3250 - Project 7
 * Author: Vicenzo Infusino
 * Instructor: Dr. Brylow
 * TA-BOT:MAILTO vicenzo.infusino@marquette.edu
 * Function to frees the blocks of memory that were being used
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

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < freelist.base)
        || ((ulong)memptr > freelist.base + freelist.bound))
    {
        return SYSERR;
    }

    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

	irqmask im;
	im = disable();

    /* TODO:
     *      + Disable interrupts
     *      + Find where the memory block should
     *        go back onto the freelist (based on address)
     *      + Find top of previous memblock
     *      + Make sure block is not overlapping on prev or next blocks
     *      + Coalesce with previous block if adjacent
     *      + Coalesce with next block if adjacent
     *      + Restore interrupts
     */
	next = (struct memblock *)freelist.head;
	prev = (struct memblock *)&freelist;
	top = NULL;

	if (next == NULL) // if statement when all of the space from freelist is allocated
	{
		freelist.head = (struct memblock *) memptr;
		freelist.head->next = NULL;
		freelist.head->length = nbytes;
		freelist.size = freelist.size + nbytes;	
		restore(im);
		return OK;
	}

	else { 
		while(next != NULL)
		{
			if(block < next) //decides if prev is before block and next is after block, otherwise iterates to the next free blocks of memory
			{
				// prev->next = block;
				block->next = next;
				block->length = nbytes;
				if (!((ulong)prev == (ulong)&freelist))
				{
					top = (ulong)prev + prev->length;
				}
				if (((block + (block->length) / sizeof(struct memblock )) > next) || (top > (ulong)block))
				{
					restore(im);
					return SYSERR; //returns error if block is overapping or overlapped
				}

				freelist.size = freelist.size + nbytes;

				if ((prev + (prev->length) / sizeof(struct memblock )) == block)//combine previous and block into one big chunk of free memory
				{
					prev->next = block->next; //previous's next block is the next block of free space
					prev->length += block->length; //previous becomes the size of previous + block
					block = prev; //change address position of block to prev so block can be used in the following conditional
				}
				else
				{
					prev->next = block;
				}
				if ((block + (block->length) / sizeof(struct memblock )) == next) //combine next and block into one large free memory block
				{
					block->next = next->next; //block's end is now next's end
					block->length += next->length; //block is now the size of block + next
				}
				restore(im);
				return OK;
			}
		prev = next;
		next = next->next;
		}
	}
	restore(im);
	return OK;
}
