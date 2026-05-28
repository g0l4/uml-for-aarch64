/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __FAULTINFO_ARM64_H
#define __FAULTINFO_ARM64_H

/* Fault info for arm64 -- derived from the ESR record in the signal frame. */
struct faultinfo {
	int error_code;		/* ESR_EL1 syndrome, extracted by the stub */
	unsigned long addr;	/* FAR_EL1 (faulting virtual address) */
	int trap_no;		/* ESR exception class */
};

#define ARM64_ESR_EC_SHIFT	26
#define ARM64_ESR_EC(esr)	(((esr) >> ARM64_ESR_EC_SHIFT) & 0x3f)
#define ARM64_ESR_WNR		(1 << 6)

/* Instruction/Data Abort from a lower EL (userspace). */
#define ARM64_ESR_EC_IABT_EL0	0x20
#define ARM64_ESR_EC_DABT_EL0	0x24

#define FAULT_WRITE(fi)		((fi).trap_no == ARM64_ESR_EC_DABT_EL0 && \
				 ((fi).error_code & ARM64_ESR_WNR))
#define FAULT_ADDRESS(fi)	((fi).addr)

#define SEGV_IS_FIXABLE(fi)	((fi)->trap_no == ARM64_ESR_EC_IABT_EL0 || \
				 (fi)->trap_no == ARM64_ESR_EC_DABT_EL0)

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
