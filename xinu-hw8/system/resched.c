/**
 * @file resched.c
 * @provides resched
 * COSC 3250 Assignment 6
 * Author: Vicenzo Infusino and Klemenes Koszarek
 * Instructor: Dr. Brylow
 * TA-BOT:MAILTO vicenzo.infusino@marquette.edu klemens.koszarek@marquette.edu
 * Modified create file to incorporate the tickets field.
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

extern void ctxsw(void *, void *);

/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */

/* Helper function to help get the total number of tickets from all
prcoesses. */

int lotteryScheduling(void)
{
	unsigned int total = 0;
	int count = 0;
	int i;

	for (i = 0; i < NPROC; i++) //gets total number of tickets for all current processes
	{
		if ((proctab[i].state == PRREADY) || (proctab[i].state == PRCURR)) //getting the tickets from processes in ready state
		{
			total += proctab[i].tickets; //incrementing the amount of tickets
 		}
	}
	unsigned int winningTicket;
	winningTicket = random(total); //picks a random ticket from the total number of tickets
	for (i = 0; i < NPROC; i++)
	{
		if ((proctab[i].state == PRREADY) || (proctab[i].state == PRCURR))
		{
			count += proctab[i].tickets;
			if(count > winningTicket) //if the count is greater than the winning ticket, stop and return the winning ticket
			{
				break;
			}
		}
	}
	return i; //returns the winner out of the break statement
}

/* Above function also picks a random ticket using random()
and traverses through the process table to identify which process
has the random ticket value */

syscall resched(void)
{
	irqmask im;
	im = disable();

	pcb *oldproc;               /* pointer to old process entry */
 	pcb *newproc;               /* pointer to new process entry */
 	oldproc = &proctab[currpid];
	

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid, readylist);
    }

    /**
     * We recommend you use a helper function for the following:
     * TODO: Get the total number of tickets from all processes that are in current and ready states.
     * Utilize the random() function to pick a random ticket.
     * Traverse through the process table to identify which proccess has the random ticket value.
     * Remove process from queue.
     * Set currpid to the new process.
     */

	currpid = lotteryScheduling(); //call lottery scheduling method
	remove(currpid);
    	newproc = &proctab[currpid];
    	newproc->state = PRCURR;    /* mark it currently running    */


#if PREEMPT
    preempt = QUANTUM;
#endif

	ctxsw(&oldproc->stkptr, &newproc->stkptr);

	restore(im);

	/* The OLD process returns here when resumed. */
	return OK;
}
