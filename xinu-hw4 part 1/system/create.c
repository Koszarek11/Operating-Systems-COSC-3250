/**
 * @file create.c
 * @provides create, newpid, userret
 * Name: Vinny Infusino and Klemens Koszarek
 * Instructor: Dr. Brylow
 * COSC 3250 Assignment 4
 * This function sets up PCB entry, initializes process context, and
 * places arguments into activation records.
 * TA-BOT:MAILTO vicenzo.infusino@marquette.edu klemens.koszarek@marquette.edu
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, char *name, ulong nargs, ...)
{
    ulong *saddr;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */

    if (ssize < MINSTK)
        ssize = MINSTK;
    ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;
    /* round up to even boundary    */
    saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
    pid = newpid();
    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }

    numproc++;
    ppcb = &proctab[pid];

	// TODO: Setup PCB entry for new process.

	ppcb -> stkbase = (ulong*)(((ulong)saddr)-ssize);
	ppcb -> stklen = ssize; //Initializing stklen to the size of the stack
	ppcb -> stkptr = NULL; //Sets stack pointer to null
	ppcb -> state = PRSUSP; //Sets state to Suspended
	strncpy(ppcb->name, name, PNMLEN); //calls built in strncpy function, starts at destination and
	// copies up to the length of the process name from the sring pointed to by name.

    /* Initialize stack with accounting block. */
    *saddr = STACKMAGIC;
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    /* Handle variable number of arguments passed to starting function   */
    if (nargs)
    {
        pads = ((nargs - 1) / 4) * 4;
    }
    /* If more than 4 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }

	// TODO: Initialize process context.
  //Loop to run the length of program counter
	for (i = 0; i <= CTX_PC; i++)
	{
    //sets stack pointer to 0
		*--saddr = 0;
	}

  //pushes the number of the current running process into link register
	saddr[CTX_LR] = (int) userret;
  //address of next process pushed to program counter
	saddr[CTX_PC] = (int) funcaddr;

  //sets up stackpointer to stack address
	ppcb->stkptr = saddr;

	// TODO:  Place arguments into activation record.
	//        See K&R 7.3 for example using va_start, va_arg and
	//        va_end macros for variable argument functions.

  //method passing ap and that number of arguments
	va_start(ap, nargs);

  //continues on # of arguments
	for (i = 0; i < nargs; i++)
	{
		if (i < 4)
		{
      //pushes values into address of stack
			saddr[i] = va_arg(ap, ulong);
		}
		else
		{
      //pushes vlaues into address of stack
			*(saddr + (i + 12)) = va_arg(ap, ulong);
		}
	}
  //passing ap through method va_end
	va_end(ap);

    //return process for id
    return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++)
    {                           /* check all NPROC slots    */
        nextpid = (nextpid + 1) % NPROC;
        if (PRFREE == proctab[nextpid].state)
        {
            return nextpid;
        }
    }
    return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
    kill(currpid);
}
