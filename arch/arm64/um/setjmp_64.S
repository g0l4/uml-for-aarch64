/* SPDX-License-Identifier: GPL-2.0 */
/*
 * setjmp/longjmp for arm64 UML.
 *
 * The jmp_buf layout (see archsetjmp.h):
 *   offset 0:   x19
 *   offset 8:   x20
 *   offset 16:  x21
 *   offset 24:  x22
 *   offset 32:  x23
 *   offset 40:  x24
 *   offset 48:  x25
 *   offset 56:  x26
 *   offset 64:  x27
 *   offset 72:  x28
 *   offset 80:  x29 (FP)
 *   offset 88:  SP (post-return)
 *   offset 96:  x30 (LR / return address)
 */

	.text
	.align 2
	.globl kernel_setjmp
	.type kernel_setjmp, @function
kernel_setjmp:
	mov	x9, x30			/* Save return address */
	mov	x10, sp			/* Save SP (pre-call, since x30 pushes nothing on arm64) */
	mov	w0, #0			/* Return value = 0 */
	stp	x19, x20, [x0, #0]
	stp	x21, x22, [x0, #16]
	stp	x23, x24, [x0, #32]
	stp	x25, x26, [x0, #48]
	stp	x27, x28, [x0, #64]
	stp	x29, x10, [x0, #80]	/* FP, SP */
	str	x9,  [x0, #96]		/* LR (return address) */
	ret

	.size kernel_setjmp, .-kernel_setjmp

	.text
	.align 2
	.globl kernel_longjmp
	.type kernel_longjmp, @function
kernel_longjmp:
	mov	w0, w1			/* Return value (int) */
	ldp	x19, x20, [x0, #0]
	ldp	x21, x22, [x0, #16]
	ldp	x23, x24, [x0, #32]
	ldp	x25, x26, [x0, #48]
	ldp	x27, x28, [x0, #64]
	ldp	x29, x9,  [x0, #80]	/* FP, saved SP */
	ldr	x10, [x0, #96]		/* saved LR */
	mov	sp, x9
	br	x10

	.size kernel_longjmp, .-kernel_longjmp
