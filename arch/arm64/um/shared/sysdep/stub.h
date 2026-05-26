/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SYSDEP_STUB_ARM64_H
#define __SYSDEP_STUB_ARM64_H

#include <asm/unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <as-layout.h>
#include <stub-data.h>
#include <sysdep/ptrace_user.h>
#include <linux/stddef.h>

#define STUB_MMAP_NR __NR_mmap
#define MMAP_OFFSET(o) (o)

/*
 * Arm64 syscall convention:
 *   NR in x8, return in x0, clobbered: x9-x17 (but x16/x17 are
 *   also used by the veneer). The kernel preserves x0-x17.
 *   We list x9-x15 as clobber so the compiler reloads after a syscall.
 */
#define __syscall_clobber "x9","x10","x11","x12","x13","x14","x15","x16","x17","cc","memory"
#define __syscall "svc #0"

static __always_inline long stub_syscall0(long syscall)
{
	register long x8 __asm__("x8") = syscall;
	register long x0 __asm__("x0");

	__asm__ volatile (__syscall
		: "=r" (x0)
		: "r" (x8)
		: __syscall_clobber);

	return x0;
}

static __always_inline long stub_syscall1(long syscall, long arg1)
{
	register long x8 __asm__("x8") = syscall;
	register long x0 __asm__("x0") = arg1;

	__asm__ volatile (__syscall
		: "+r" (x0)
		: "r" (x8)
		: __syscall_clobber);

	return x0;
}

static __always_inline long stub_syscall2(long syscall, long arg1, long arg2)
{
	register long x8 __asm__("x8") = syscall;
	register long x0 __asm__("x0") = arg1;
	register long x1 __asm__("x1") = arg2;

	__asm__ volatile (__syscall
		: "+r" (x0)
		: "r" (x8), "r" (x1)
		: __syscall_clobber);

	return x0;
}

static __always_inline long stub_syscall3(long syscall, long arg1, long arg2,
					  long arg3)
{
	register long x8 __asm__("x8") = syscall;
	register long x0 __asm__("x0") = arg1;
	register long x1 __asm__("x1") = arg2;
	register long x2 __asm__("x2") = arg3;

	__asm__ volatile (__syscall
		: "+r" (x0)
		: "r" (x8), "r" (x1), "r" (x2)
		: __syscall_clobber);

	return x0;
}

static __always_inline long stub_syscall4(long syscall, long arg1, long arg2,
					  long arg3, long arg4)
{
	register long x8 __asm__("x8") = syscall;
	register long x0 __asm__("x0") = arg1;
	register long x1 __asm__("x1") = arg2;
	register long x2 __asm__("x2") = arg3;
	register long x3 __asm__("x3") = arg4;

	__asm__ volatile (__syscall
		: "+r" (x0)
		: "r" (x8), "r" (x1), "r" (x2), "r" (x3)
		: __syscall_clobber);

	return x0;
}

static __always_inline long stub_syscall5(long syscall, long arg1, long arg2,
					  long arg3, long arg4, long arg5)
{
	register long x8 __asm__("x8") = syscall;
	register long x0 __asm__("x0") = arg1;
	register long x1 __asm__("x1") = arg2;
	register long x2 __asm__("x2") = arg3;
	register long x3 __asm__("x3") = arg4;
	register long x4 __asm__("x4") = arg5;

	__asm__ volatile (__syscall
		: "+r" (x0)
		: "r" (x8), "r" (x1), "r" (x2), "r" (x3), "r" (x4)
		: __syscall_clobber);

	return x0;
}

static __always_inline long stub_syscall6(long syscall, long arg1, long arg2,
					  long arg3, long arg4, long arg5,
					  long arg6)
{
	register long x8 __asm__("x8") = syscall;
	register long x0 __asm__("x0") = arg1;
	register long x1 __asm__("x1") = arg2;
	register long x2 __asm__("x2") = arg3;
	register long x3 __asm__("x3") = arg4;
	register long x4 __asm__("x4") = arg5;
	register long x5 __asm__("x5") = arg6;

	__asm__ volatile (__syscall
		: "+r" (x0)
		: "r" (x8), "r" (x1), "r" (x2), "r" (x3), "r" (x4), "r" (x5)
		: __syscall_clobber);

	return x0;
}

static __always_inline void trap_myself(void)
{
	/* BRK #0x01 is the arch breakpoint on arm64 */
	__asm__("brk #0x01");
}

static __always_inline void *get_stub_data(void)
{
	unsigned long ret;

	asm volatile (
		"adr %0, 0f\n"
		"0:\n"
		"and %0, %0, %1\n"
		"add %0, %0, %2\n"
		: "=&r" (ret)
		: "r" (~(UM_KERN_PAGE_SIZE - 1UL)),
		  "r" (UM_KERN_PAGE_SIZE));

	return (void *)ret;
}

/*
 * Jump to the stub function with a known stack.
 *
 * Arm64 requires a valid stack VMA at the target sp.  The stub
 * binary loads at a low address (0x400000) but the kernel places
 * the initial stack VMA at the top of the address space.  Moving
 * sp across the entire VA space causes SIGSEGV because the kernel
 * refuses to grow the stack VMA that far.  Allocate a fresh
 * MAP_GROWSDOWN region first.
 */
#define stub_start(fn)							\
	do {								\
		unsigned long __sp, __sp_base;				\
		register long __x8 asm("x8") = __NR_mmap;		\
		register long __x0 asm("x0") = 0;			\
		register long __x1 asm("x1") = STUB_SIZE;		\
		register long __x2 asm("x2") =				\
			PROT_READ | PROT_WRITE;				\
		register long __x3 asm("x3") =				\
			MAP_PRIVATE | MAP_ANONYMOUS | MAP_GROWSDOWN;	\
		register long __x4 asm("x4") = -1;			\
		register long __x5 asm("x5") = 0;			\
		asm volatile ("svc #0"					\
			: "+r" (__x0)					\
			: "r" (__x8), "r" (__x1), "r" (__x2),		\
			  "r" (__x3), "r" (__x4), "r" (__x5)		\
			: "x9","x10","x11","x12","x13","x14","x15",	\
			  "x16","x17","cc","memory");			\
		__sp_base = __x0;					\
		__sp = __sp_base + STUB_SIZE;				\
		asm volatile (						\
			"mov sp, %0\n"					\
			"blr %1\n"					\
			:: "r" (__sp), "r" (&(fn)));			\
	} while (0)

static __always_inline void
stub_seccomp_restore_state(struct stub_data_arch *arch)
{
	/* tpidr_el0 is accessible from EL0, write it directly */
	if (arch->sync & STUB_SYNC_TPIDR)
		asm volatile ("msr tpidr_el0, %0" :: "r" (arch->tpidr_el0));

	arch->sync = 0;
}

extern void stub_segv_handler(int, siginfo_t *, void *);
extern void stub_syscall_handler(void);
extern void stub_signal_interrupt(int, siginfo_t *, void *);
extern void stub_signal_restorer(void);

#endif
