// SPDX-License-Identifier: GPL-2.0
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <registers.h>
#include <os.h>

/*
 * Arm64 uses tpidr_el0 for TLS. There is no arch_prctl equivalent
 * on native arm64 (TLS is set via the ELF loader DTV), but the
 * UML infrastructure may call this. We store the value in a
 * dedicated register slot.
 */

void arch_switch_to(struct task_struct *to)
{
	/*
	 * On arm64, tpidr_el0 is saved/restored through the
	 * ptrace register set, so nothing extra is needed here.
	 */
}

SYSCALL_DEFINE6(mmap, unsigned long, addr, unsigned long, len,
		unsigned long, prot, unsigned long, flags,
		unsigned long, fd, unsigned long, off)
{
	if (off & ~PAGE_MASK)
		return -EINVAL;

	return ksys_mmap_pgoff(addr, len, prot, flags, fd, off >> PAGE_SHIFT);
}

/*
 * UML on arm64 is 64-bit only. 32-bit compat is not supported.
 */
