// SPDX-License-Identifier: GPL-2.0
#include <linux/personality.h>
#include <linux/ptrace.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <linux/uaccess.h>
#include <asm/ucontext.h>
#include <frame_kern.h>
#include <registers.h>
#include <skas.h>
#include <linux/regset.h>
#include <asm/sigcontext.h>
#include <as-layout.h>
#include <asm/sections.h>

extern void stub_signal_restorer(void);

/*
 * Arm64 signal frame layout.
 *
 * The kernel delivers signals with a full rt_sigframe on the stack:
 *
 *   high addr
 *   [fpsimd_context / sve_context / extra records ...]
 *   [terminator _aarch64_ctx]
 *   [struct rt_sigframe]
 *   [struct siginfo]           (only for SA_SIGINFO)
 *   low addr (sp after signal delivery)
 *
 * struct rt_sigframe {
 *     struct siginfo info;
 *     struct ucontext uc;
 * };
 *
 * struct ucontext {
 *     unsigned long uc_flags;
 *     struct ucontext *uc_link;
 *     stack_t uc_stack;
 *     sigset_t uc_sigmask;
 *     // arm64 aligns sigcontext to 16 bytes
 *     struct sigcontext uc_mcontext;
 * };
 *
 * The sigcontext.regs[31] / sp / pc / pstate hold the GP state.
 * FP/SIMD state lives in fpsimd_context inside __reserved[].
 */

struct rt_sigframe {
	struct siginfo info;
	struct ucontext uc;
};

/*
 * Compute the full signal frame size including all record allocations.
 * Returns the total size needed before the sigframe on the stack.
 */
static unsigned long sigframe_size(void)
{
	return round_up(sizeof(struct rt_sigframe), 16);
}

static int copy_sc_from_user(struct pt_regs *regs,
			     struct sigcontext __user *from)
{
	struct sigcontext sc;
	int i, err;

	current->restart_block.fn = do_no_restart_syscall;

	err = __copy_from_user(&sc, from, sizeof(sc));
	if (err)
		return err;

	/* GP registers */
	for (i = 0; i < 31; i++)
		regs->regs.gp[i] = sc.regs[i];

	regs->regs.gp[HOST_SP] = sc.sp;
	regs->regs.gp[HOST_PC] = sc.pc;
	regs->regs.gp[HOST_PSTATE] = sc.pstate;

	return 0;
}

static int copy_sc_to_user(struct sigcontext __user *to,
			   struct pt_regs *regs,
			   unsigned long mask)
{
	struct sigcontext sc;
	struct faultinfo *fi = &current->thread.arch.faultinfo;
	int err;
	int i;

	memset(&sc, 0, sizeof(sc));

	sc.fault_address = fi->addr;

	for (i = 0; i < 31; i++)
		sc.regs[i] = regs->regs.gp[i];

	sc.sp = regs->regs.gp[HOST_SP];
	sc.pc = regs->regs.gp[HOST_PC];
	sc.pstate = regs->regs.gp[HOST_PSTATE];

	err = __copy_to_user(to, &sc, sizeof(sc));
	if (err)
		return err;

	return 0;
}

int setup_signal_stack_si(unsigned long stack_top, struct ksignal *ksig,
			  struct pt_regs *regs, sigset_t *set)
{
	struct rt_sigframe __user *frame;
	unsigned long __user *frame_record;
	int err = 0, sig = ksig->sig;
	unsigned long frame_record_addr;
	unsigned long sigtramp;
	unsigned long old_fp, old_lr;

	frame_record_addr = round_down(stack_top - 2 * sizeof(unsigned long), 16);
	frame_record = (unsigned long __user *)frame_record_addr;

	/*
	 * Arm64 requires 16-byte SP alignment at public interfaces.
	 * The signal frame itself is 16-byte aligned through struct layout.
	 */
	frame = (struct rt_sigframe __user *)
		round_down(frame_record_addr - sigframe_size(), 16);

	if (!access_ok(frame, frame_record_addr + 2 * sizeof(unsigned long) -
		       (unsigned long)frame))
		goto out;

	old_fp = UPT_X29(&regs->regs);
	old_lr = UPT_X30(&regs->regs);

	if (ksig->ka.sa.sa_flags & SA_SIGINFO) {
		err |= copy_siginfo_to_user(&frame->info, &ksig->info);
		if (err)
			goto out;
	}

	/* Create the ucontext */
	err |= __put_user(0, &frame->uc.uc_flags);
	err |= __put_user(NULL, &frame->uc.uc_link);
	err |= __save_altstack(&frame->uc.uc_stack, PT_REGS_SP(regs));

	err |= copy_sc_to_user(&frame->uc.uc_mcontext, regs,
			       set->sig[0]);

	err |= __copy_to_user(&frame->uc.uc_sigmask, set, sizeof(*set));
	err |= __put_user(old_fp, &frame_record[0]);
	err |= __put_user(old_lr, &frame_record[1]);

	if (err)
		return err;

	/*
	 * Set up to return from userspace. The sa_restorer is a
	 * trampoline provided by userspace that calls rt_sigreturn.
	 */
	if (ksig->ka.sa.sa_flags & SA_RESTORER)
		sigtramp = (unsigned long)ksig->ka.sa.sa_restorer;
	else
		sigtramp = STUB_CODE + (unsigned long)stub_signal_restorer -
			   (unsigned long)__syscall_stub_start;

	PT_REGS_SP(regs) = (unsigned long)frame;
	UPT_X29(&regs->regs) = frame_record_addr;
	UPT_X30(&regs->regs) = sigtramp;

	/* A64 calling convention: x0 = signal number */
	UPT_X0(&regs->regs) = sig;

	/* If SA_SIGINFO, x1 = siginfo ptr, x2 = ucontext ptr */
	if (ksig->ka.sa.sa_flags & SA_SIGINFO) {
		UPT_X1(&regs->regs) = (unsigned long)&frame->info;
		UPT_X2(&regs->regs) = (unsigned long)&frame->uc;
	}

	UPT_IP(&regs->regs) = (unsigned long)ksig->ka.sa.sa_handler;
out:
	return err;
}

SYSCALL_DEFINE0(rt_sigreturn)
{
	unsigned long sp = PT_REGS_SP(&current->thread.regs);
	struct rt_sigframe __user *frame =
		(struct rt_sigframe __user *)sp;
	struct ucontext __user *uc = &frame->uc;
	sigset_t set;

	if (copy_from_user(&set, &uc->uc_sigmask, sizeof(set)))
		goto segfault;

	set_current_blocked(&set);

	if (copy_sc_from_user(&current->thread.regs, &uc->uc_mcontext))
		goto segfault;

	/* Avoid ERESTART handling */
	PT_REGS_SYSCALL_NR(&current->thread.regs) = -1;
	return PT_REGS_SYSCALL_RET(&current->thread.regs);

segfault:
	force_sig(SIGSEGV);
	return 0;
}
