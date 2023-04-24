/**
 * @file     ctxsw.s
 * @provides ctxsw
 * Name: Vinny Infusino and Klemens Koszarek
 * Instructor: Dr. Brylow
 * COSC 3250 / COEN 4820 Assignment 4
 * TA-BOT:MAILTO vicenzo.infusino@marquette.edu klemens.koszarek@marquette.edu
 * This function is our actual context switch function where we store and load
 * registers in assembly.
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */

#include <arm.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldstack, &newstack)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
ctxsw:
	.func ctxsw
// TODO: Save process state, including at least the
//	callee-save ("non-volatile") registers.

  SUB sp, sp, #64

	// STR is used to store something from reigster to memory address
	STR r0,[sp, #CTX_R0 * 4]
	STR r1,[sp, #CTX_R1 * 4]
	STR r2,[sp, #CTX_R2 * 4]
	STR r3,[sp, #CTX_R3 * 4]
	STR r4,[sp, #CTX_R4 * 4] //storing the non-volatile memory registers
	STR r5,[sp, #CTX_R5 * 4]
	STR r6,[sp, #CTX_R6 * 4]
	STR r7,[sp, #CTX_R7 * 4]
	STR r8,[sp, #CTX_R8 * 4]
	STR r9,[sp, #CTX_R9 * 4]
	STR r10,[sp, #CTX_R10 * 4]
	STR r11,[sp, #CTX_R11 * 4]
	STR r14,[sp, #CTX_LR * 4]
	STR r14,[sp, #CTX_PC * 4]

  mrs r14,cpsr

  STR r14,[sp, #CTX_IP * 4]


	STR sp, [r0]

// TODO: Restore process state, including at least the
//	callee-save ("non-volatile") registers.
//	These call and restore segments must match the register data
//	layout you choose in create()

	LDR sp, [r1]

	// LDR loads something from memory into a register
	LDR r0,[sp, #CTX_R0 * 4]
	LDR r1,[sp, #CTX_R1 * 4]
	LDR r2,[sp, #CTX_R2 * 4]
	LDR r3,[sp, #CTX_R3 * 4]
	LDR r4,[sp, #CTX_R4 * 4]
	LDR r5,[sp, #CTX_R5 * 4]
	LDR r6,[sp, #CTX_R6 * 4]
	LDR r7,[sp, #CTX_R7 * 4]
	LDR r8,[sp, #CTX_R8 * 4]
	LDR r9,[sp, #CTX_R9 * 4]
	LDR r10,[sp, #CTX_R10 * 4]
	LDR r11,[sp, #CTX_R11 * 4] //saving to register

  LDR r14,[sp, #CTX_IP *4]
  msr cpsr_c, r14



// TODO: Jump to next function.
//	Note: there are two important values at play here --
//	the address of the process's function that we want to end up in
//	once we leave this ctxsw(), and the value we want to have in the
//	link register.  Those are almost always the same value, except
//	for the very first time we ctxsw() to a new process, when they
//	are different.  See create() for more details.

	LDR r14,[sp, #CTX_LR * 4] //Link Register to where old program finishes.
	LDR r12,[sp, #CTX_PC * 4] //next instruction for beginning of next function

  ADD sp,sp, #64

  bx r12

// Note: This bx lr line immediately returns from whence we came.  This
//	is here as a placeholder so that you don't crash your O/S when
//	you call ctxsw() before it is complete.  But, this line of code
//	is not actually a part of a completed solution.

	.end ctxsw
