/**
 * @file testcases.c
 * @provides testcases
 * Author: Vicenzo Infusino and Klemens Koszarek
 * Instructor: Dr. Brylow
 * Project 7  - Operating Systems
 * TA-BOT:MAILTO vicenzo.infusino@marquette.edu klemens.koszarek@marquette.edu
 * This is the testcases.c file for the memory management project
 * file for project 7.
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

/**
 * testcases - called after initialization completes to test things.
 */

void printFreeList(void)
{
	memblk *p = freelist.head;
	while (p != NULL)
	{
		kprintf("[0x%08X : %10d : 0x%08x]\r\n", p, p->length, p->next);
		p = p -> next;
	}
}

void testcases(void)
{
    int c;

	kprintf("1.) Printing the Freelist Testcase\r\n");
	kprintf("A.) Testing the Addresses with Getmem\r\n");
	kprintf("B.) Testing the Addresses with Malloc\r\n");
	kprintf("2.) Printing the Freelist Testcase with Freemem\r\n");
	kprintf("3.) Creating Freeblock in between middle of allocated blocks\r\n");
	kprintf("4.) Creating Freeblock in the beginning and end of allocated block\r\n");
	kprintf("5.) Using malloc instead of the getmem function\r\n");
	kprintf("6.) Uses both malloc and free instead of getmem and freemem\r\n");
	kprintf("7.) Uses both malloc and free with 80 free spaces out in front\r\n");
	kprintf("8.) Testing the size of the memblock\r\n");
	kprintf("9.) Allocating and Freeing the entire freelist size\r\n");
	kprintf("===TEST BEGIN===\r\n");
	c = kgetc();
	void * pmem, * pmem1, * pmem2;
	switch (c)
    	{
	case '1':
		printFreeList();
		getmem(32);
		printFreeList();
		getmem(32);
		printFreeList();
		getmem(32);
		printFreeList();
		break;
	case 'A':
		pmem = getmem(32);
		kprintf("Address was %d\r\n", ((int) pmem) - (int) memheap);
		printFreeList();
		break;
	case 'B':
		pmem = malloc(32);
		kprintf("Address was %d\r\n", ((int) pmem) - (int) memheap);
		printFreeList();
		break;
	case '2':
		printFreeList();
		pmem = getmem(32);
		printFreeList();
		freemem(pmem, 32); //will printFreeList out twice and both equalling each other
		printFreeList();
		break;
	case '3':
		printFreeList();
		pmem = getmem(32);
		pmem1 = getmem(64);
		pmem2 = getmem(128);
		freemem(pmem1, 64); //will print out free list in the beginning, and the one for 64, and the remaining one
		printFreeList();
		break;
	case '4':
		printFreeList();
		pmem2 = getmem(8);
		pmem1 = getmem(16);
		pmem = getmem(32);
		freemem(pmem2, 8);
		freemem(pmem,32);
		printFreeList(); //will print out block for 8, and then 32 + remaining one.
		break;
	case '5':
		printFreeList();
		pmem = malloc(32); //using malloc instead of getmem - malloc adds 8 more for accounting info
		printFreeList();
		break;
	case '6':
		printFreeList();
		pmem = malloc(32);
		printFreeList();
		free(pmem); //allocates 32 spaces and then freemem will occupy those 32 spaces and return original value
		printFreeList();
		break;
	case '7':
		printFreeList();
		pmem = malloc(32);
		pmem1 = malloc(32);
		pmem2 = malloc(32); //frees 80 out in front and then a 40 gap, and then returns the rest
		printFreeList();
		free(pmem);
		free(pmem1);
		printFreeList();
		break;
	case '8':
		kprintf("%d 0x%X\r\n", sizeof(struct memblock *), sizeof(struct memblock *));
		kprintf("%d 0x%X\r\n", sizeof(struct memblock), sizeof(struct memblock));
		break;
	case '9':
		kprintf("Before\n");
		printFreeList();
		int size;
		size = freelist.size;
		pmem = getmem(size);
		kprintf("After getmem\n");
		printFreeList();
		freemem(pmem, size);
		kprintf("after free\n");
		printFreeList();
    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
