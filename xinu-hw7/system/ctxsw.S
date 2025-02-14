/**
 * @file     ctxsw.s
 * @provides ctxsw
 * Name: Vinny Infusino and Klemens Koszarek
 * Instructor: Dr. Brylow
 * COSC 3250 / COEN 4820 Assignment 4 - modified for 6
 * TA-BOT:MAILTO vicenzo.infusino@marquette.edu klemens.koszarek@marquette.edu
 * This function is our actual context switch function where we store and load
 * registers in assembly. Also, this sets up user processes to run with interrupts
 * on by default.
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

	SUB sp, sp, #64 //pushes 16 register spaces onto current stack

	// STR is used to store something from reigster to memory address
	STR r0,[sp, #CTX_R0 * 4] //storing caller-save argument passing registers
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
	STR r14,[sp, #CTX_LR * 4] //storing the link register
	STR r14,[sp, #CTX_PC * 4] //storing the program counter

	MRS r14, cpsr
	STR r14,[sp, #CTX_SP * 4]

	STR sp, [r0] //stores stack pointer to r0

// TODO: Restore process state, including at least the
//	callee-save ("non-volatile") registers.
//	These call and restore segments must match the register data
//	layout you choose in create()

	LDR sp, [r1] //loading stack pointer to r1

	// LDR loads something from memory into a register
	LDR r0,[sp, #CTX_R0 * 4] // loading caller save argument passing registers
	LDR r1,[sp, #CTX_R1 * 4]
	LDR r2,[sp, #CTX_R2 * 4]
	LDR r3,[sp, #CTX_R3 * 4] // loading non-volatile registers (r4-r11)
	LDR r4,[sp, #CTX_R4 * 4]
	LDR r5,[sp, #CTX_R5 * 4]
	LDR r6,[sp, #CTX_R6 * 4]
	LDR r7,[sp, #CTX_R7 * 4]
	LDR r8,[sp, #CTX_R8 * 4]
	LDR r9,[sp, #CTX_R9 * 4]
	LDR r10,[sp, #CTX_R10 * 4]
	LDR r11,[sp, #CTX_R11 * 4] //saving to register

	LDR r14,[sp, #CTX_SP * 4]
	MSR cpsr_c, r14

// Jumps to the next function

	LDR r14,[sp, #CTX_LR * 4] //Link Register to where old program finishes.
	LDR r12,[sp, #CTX_PC * 4] //next instruction for beginning of next function

	ADD sp, sp, #64 //pushes 16 more registers spaces off the stack

// Note: This bx lr line immediately returns from whence we came.  This
//	is here as a placeholder so that you don't crash your O/S when
//	you call ctxsw() before it is complete.  But, this line of code
//	is not actually a part of a completed solution.

	bx	r12

	.end ctxsw
