// SPDX-License-Identifier: GPL-2.0
#include <stddef.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <linux/elf.h>
#include <sysdep/tls.h>
#include <registers.h>

/*
 * Arm64 uses tpidr_el0 (x18) for the TLS base pointer.
 * Unlike x86's arch_prctl, arm64 allows EL0 to read/write tpidr_el0
 * directly via MRS/MSR. For UML, we store it in the register file.
 */

int os_set_thread_area(user_desc_t *info, int pid)
{
	unsigned long tpidr = *info;
	struct user_regs_struct regs;
	struct iovec iov = {
		.iov_base = &regs,
		.iov_len = sizeof(regs),
	};

	if (ptrace(PTRACE_GETREGSET, pid, NT_PRSTATUS, &iov) < 0)
		return -errno;

	regs.regs[18] = tpidr;

	if (ptrace(PTRACE_SETREGSET, pid, NT_PRSTATUS, &iov) < 0)
		return -errno;

	return 0;
}

int os_get_thread_area(user_desc_t *info, int pid)
{
	struct user_regs_struct regs;
	struct iovec iov = {
		.iov_base = &regs,
		.iov_len = sizeof(regs),
	};

	if (ptrace(PTRACE_GETREGSET, pid, NT_PRSTATUS, &iov) < 0)
		return -errno;

	*info = regs.regs[18];
	return 0;
}
