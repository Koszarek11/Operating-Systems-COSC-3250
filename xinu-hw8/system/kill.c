/**
 * @file kill.c
 * Provides: kill
 *
 * COSC 3250 Assignment 8
 */

/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/*
 * kill  --  kill a process and remove it from the system
 */
syscall kill(int pid)
{
    pcb *ppcb;                  /* points to process control block for pid */

    if (isbadpid(pid) || (0 == pid)
        || (PRFREE == (ppcb = &proctab[pid])->state))
    {
        return SYSERR;
    }

    ppcb = &proctab[pid];

    --numproc;

   freemem(ppcb->stkbase, ppcb->stklen);

    while (nonempty(ppcb->joinQueue))
    {
	ready(dequeue(ppcb->joinQueue), RESCHED_NO);
    }

    switch (ppcb->state)
    {
    case PRCURR:
        ppcb->state = PRFREE;   /* suicide */
	resched();

    case PRREADY:
    case PRJOIN:
        remove(pid);

    default:
        ppcb->state = PRFREE;
    }

    return OK;
}
