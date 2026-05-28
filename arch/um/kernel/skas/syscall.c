// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 */

#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <linux/seccomp.h>
#include <kern_util.h>
#include <sysdep/ptrace.h>
#include <sysdep/ptrace_user.h>
#include <linux/time-internal.h>
#include <asm/syscall.h>
#include <asm/unistd.h>
#include <asm/delay.h>

void handle_syscall(struct uml_pt_regs *r)
{
	struct pt_regs *regs = container_of(r, struct pt_regs, regs);
	int syscall;
	unsigned long arg1, arg2, arg3, arg4, arg5, arg6;

	/* Initialize the syscall number. */
	UPT_SYSCALL_NR(r) = PT_SYSCALL_NR(r->gp);

	if (syscall_trace_enter(regs))
		goto out_set_errno;

	/* Do the seccomp check after ptrace; failures should be fast. */
	if (secure_computing() == -1)
		goto out;

	syscall = UPT_SYSCALL_NR(r);
	arg1 = UPT_SYSCALL_ARG1(&regs->regs);
	arg2 = UPT_SYSCALL_ARG2(&regs->regs);
	arg3 = UPT_SYSCALL_ARG3(&regs->regs);
	arg4 = UPT_SYSCALL_ARG4(&regs->regs);
	arg5 = UPT_SYSCALL_ARG5(&regs->regs);
	arg6 = UPT_SYSCALL_ARG6(&regs->regs);

	PT_REGS_SET_SYSCALL_RETURN(regs, -ENOSYS);

	/*
	 * If no time passes, then sched_yield may not actually yield, causing
	 * broken spinlock implementations in userspace (ASAN) to hang for long
	 * periods of time.
	 */
	if ((time_travel_mode == TT_MODE_INFCPU ||
	     time_travel_mode == TT_MODE_EXTERNAL) &&
	    syscall == __NR_sched_yield)
		tt_extra_sched_jiffies += 1;

	if (syscall >= 0 && syscall < __NR_syscalls) {
		unsigned long ret;

		ret = (*sys_call_table[syscall])(arg1, arg2, arg3, arg4,
						 arg5, arg6);

		PT_REGS_SET_SYSCALL_RETURN(regs, ret);

		/*
		 * An error value here can be some form of -ERESTARTSYS
		 * and then we'd just loop. Make any error syscalls take
		 * some time, so that it won't just loop if something is
		 * not ready, and hopefully other things will make some
		 * progress.
		 */
		if (IS_ERR_VALUE(ret) &&
		    (time_travel_mode == TT_MODE_INFCPU ||
		     time_travel_mode == TT_MODE_EXTERNAL)) {
			um_udelay(1);
			schedule();
		}
	}

out:
	syscall_trace_leave(regs);
	return;

out_set_errno:
	PT_REGS_SET_SYSCALL_RETURN(regs, -ENOSYS);
	goto out;
}
