/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ARM64_UM_SYSDEP_ARCHSETJMP_H
#define __ARM64_UM_SYSDEP_ARCHSETJMP_H

/*
 * jmp_buf layout for arm64 setjmp/longjmp.
 * We save the callee-saved registers plus SP and LR (return address).
 *
 * A64 callee-saved: x19–x28, x29 (FP), x30 (LR), plus SP.
 * x16/x17 (IP0/IP1) are intra-procedure-call scratch and don't need saving.
 */

struct __jmp_buf {
	unsigned long __x19;
	unsigned long __x20;
	unsigned long __x21;
	unsigned long __x22;
	unsigned long __x23;
	unsigned long __x24;
	unsigned long __x25;
	unsigned long __x26;
	unsigned long __x27;
	unsigned long __x28;
	unsigned long __fp;	/* x29 - frame pointer */
	unsigned long __sp;	/* stack pointer */
	unsigned long __lr;	/* x30 - link register (return address) */
};

typedef struct __jmp_buf jmp_buf[1];

#define JB_IP __lr
#define JB_SP __sp

/*
 * Unlike x86, AArch64 keeps the return address in LR rather than pushing it
 * onto the stack, so a fabricated function entry SP must stay 16-byte aligned.
 */
#define JB_SP_ENTRY_OFFSET 0

unsigned long get_thread_reg(int reg, jmp_buf *buf);

#endif
