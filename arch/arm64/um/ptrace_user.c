// SPDX-License-Identifier: GPL-2.0
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <linux/elf.h>
#include <ptrace_user.h>

int ptrace_getregs(long pid, unsigned long *regs_out)
{
	struct iovec iov = {
		.iov_base = regs_out,
		.iov_len = sizeof(struct user_regs_struct),
	};

	if (ptrace(PTRACE_GETREGSET, pid, NT_PRSTATUS, &iov) < 0)
		return -errno;
	return 0;
}

int ptrace_setregs(long pid, unsigned long *regs)
{
	struct iovec iov = {
		.iov_base = regs,
		.iov_len = sizeof(struct user_regs_struct),
	};

	if (ptrace(PTRACE_SETREGSET, pid, NT_PRSTATUS, &iov) < 0)
		return -errno;
	return 0;
}
