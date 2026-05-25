/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __FAULTINFO_ARM64_H
#define __FAULTINFO_ARM64_H

/* Fault info for arm64 — matches what the kernel delivers via SIGSEGV. */
struct faultinfo {
	int error_code;		/* ESR_EL1 syndrome, extracted by the stub */
	unsigned long addr;	/* FAR_EL1 (faulting virtual address) */
	int trap_no;		/* derived from esr (EC field) */
};

#define FAULT_WRITE(fi)		((fi).error_code & (1 << 6))	/* WnR bit in ESR */
#define FAULT_ADDRESS(fi)	((fi).addr)

/* Data Abort from a lower EL (userspace) */
#define ESR_EL1_EC_DABT_EL0	0x24
#define SEGV_IS_FIXABLE(fi)	(((fi)->trap_no) == ESR_EL1_EC_DABT_EL0)

#define PTRACE_FULL_FAULTINFO 1

/*
 * __get_kernel_nofault snippet: the fault address is loaded into
 * current->thread.segv_continue.
 */
#define ___backtrack_faulted(_faulted)					\
	asm volatile (							\
		"adr %0, __get_kernel_nofault_faulted_%=\n"		\
		"str %0, %1\n"						\
		"mov %0, #0\n"						\
		"b _end_%=\n"						\
		"__get_kernel_nofault_faulted_%=:\n"			\
		"mov %0, #1\n"						\
		"_end_%=:"						\
		: "=&r" (_faulted),					\
		  "=m" (current->thread.segv_continue) ::		\
	)

#endif
