/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <errno.h>
#include <ptrace_user.h>
#include <sysdep/ptrace.h>

int ptrace_getregs(long pid, unsigned long *regs_out)
{
	if (ptrace(PTRACE_GETREGS, pid, 0, regs_out) < 0)
		return -errno;
	return 0;
}

int ptrace_setregs(long pid, unsigned long *regs)
{
	if (ptrace(PTRACE_SETREGS, pid, 0, regs) < 0)
		return -errno;
	return 0;
}

int ptrace_get_thread_state(long pid, struct uml_pt_regs *regs)
{
	return ptrace_getregs(pid, regs->gp);
}

int ptrace_set_thread_state(long pid, struct uml_pt_regs *regs)
{
	return ptrace_setregs(pid, regs->gp);
}

int ptrace_get_syscall_nr(long pid, long *nr)
{
	errno = 0;
	*nr = ptrace(PTRACE_PEEKUSER, pid, PT_SYSCALL_NR_OFFSET, 0);
	if (*nr == -1 && errno)
		return -errno;
	return 0;
}

int ptrace_set_syscall_nr(long pid, long nr)
{
	if (ptrace(PTRACE_POKEUSER, pid, PT_SYSCALL_NR_OFFSET, nr) < 0)
		return -errno;
	return 0;
}

int ptrace_set_syscall_ret(long pid, unsigned long ret)
{
	if (ptrace(PTRACE_POKEUSER, pid, PT_SYSCALL_RET_OFFSET, ret) < 0)
		return -errno;
	return 0;
}
