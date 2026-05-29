// SPDX-License-Identifier: GPL-2.0
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <linux/elf.h>
#include <ptrace_user.h>
#include <sysdep/ptrace.h>

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

static int ptrace_get_tls(long pid, unsigned long *tls)
{
	struct iovec iov = {
		.iov_base = tls,
		.iov_len = sizeof(*tls),
	};

	if (ptrace(PTRACE_GETREGSET, pid, NT_ARM_TLS, &iov) < 0)
		return -errno;
	return 0;
}

static int ptrace_set_tls(long pid, unsigned long tls)
{
	struct iovec iov = {
		.iov_base = &tls,
		.iov_len = sizeof(tls),
	};

	if (ptrace(PTRACE_SETREGSET, pid, NT_ARM_TLS, &iov) < 0)
		return -errno;
	return 0;
}

int ptrace_get_thread_state(long pid, struct uml_pt_regs *regs)
{
	int err;

	err = ptrace_getregs(pid, regs->gp);
	if (err)
		return err;

	return ptrace_get_tls(pid, &regs->tpidr_el0);
}

int ptrace_set_thread_state(long pid, struct uml_pt_regs *regs)
{
	int err;

	err = ptrace_setregs(pid, regs->gp);
	if (err)
		return err;

	return ptrace_set_tls(pid, regs->tpidr_el0);
}

int ptrace_get_syscall_nr(long pid, long *nr)
{
	int syscall_nr;
	struct iovec iov = {
		.iov_base = &syscall_nr,
		.iov_len = sizeof(syscall_nr),
	};

	if (ptrace(PTRACE_GETREGSET, pid, NT_ARM_SYSTEM_CALL, &iov) < 0)
		return -errno;

	*nr = syscall_nr;
	return 0;
}

int ptrace_set_syscall_nr(long pid, long nr)
{
	int syscall_nr = nr;
	struct iovec iov = {
		.iov_base = &syscall_nr,
		.iov_len = sizeof(syscall_nr),
	};

	if (ptrace(PTRACE_SETREGSET, pid, NT_ARM_SYSTEM_CALL, &iov) < 0)
		return -errno;
	return 0;
}

int ptrace_set_syscall_ret(long pid, unsigned long ret)
{
	unsigned long regs[MAX_REG_NR];
	int err;

	err = ptrace_getregs(pid, regs);
	if (err)
		return err;

	REGS_X0(regs) = ret;

	return ptrace_setregs(pid, regs);
}
