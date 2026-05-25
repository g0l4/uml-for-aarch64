// SPDX-License-Identifier: GPL-2.0
#include <linux/linkage.h>
#include <linux/sys.h>
#include <linux/cache.h>
#include <asm/syscall.h>

extern asmlinkage long sys_ni_syscall(unsigned long, unsigned long,
				      unsigned long, unsigned long,
				      unsigned long, unsigned long);

/*
 * Handle arm64's __SYSCALL_WITH_COMPAT entries: keep the native
 * symbol and ignore the compat alias (UML on arm64 is 64-bit only).
 */
#define __SYSCALL_WITH_COMPAT(nr, sym, compat) __SYSCALL(nr, sym)

#define __SYSCALL_NORETURN __SYSCALL

#define __SYSCALL(nr, sym) extern asmlinkage long sym(unsigned long, \
	unsigned long, unsigned long, unsigned long, unsigned long, \
	unsigned long);
#include <asm/syscall_table_64.h>
#undef __SYSCALL

#define __SYSCALL(nr, sym) [nr] = sym,
#define __SYSCALL_WITH_COMPAT(nr, sym, compat) [nr] = sym,

const sys_call_ptr_t sys_call_table[] ____cacheline_aligned = {
#include <asm/syscall_table_64.h>
};

int syscall_table_size = sizeof(sys_call_table);
