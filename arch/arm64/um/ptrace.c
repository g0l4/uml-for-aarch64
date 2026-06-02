// SPDX-License-Identifier: GPL-2.0
#include <linux/sched.h>
#include <linux/ptrace.h>
#include <asm/ptrace-abi.h>
#include <registers.h>
#include <sysdep/ptrace.h>
#include <asm/ptrace.h>

/*
 * Register offsets mapping arm64 user_regs_struct to our gp[] indices.
 */
static const int reg_offsets[] = {
	[0 ... 30] = -1, /* filled below */
	[HOST_SP] = 31,
	[HOST_PC] = 32,
	[HOST_PSTATE] = 33,
};

int putreg(struct task_struct *child, int regno, unsigned long value)
{
	unsigned long *gp = child->thread.regs.regs.gp;

	switch (regno) {
	/* x0-x30 + SP + PC + PSTATE (the base user_regs_struct) */
	case 0 ... 33:
		gp[regno] = value;
		break;

	case 34: /* orig_x0 / syscall number slot */
		UPT_SYSCALL_NR(&child->thread.regs.regs) = value;
		break;

	case 35: /* PSTATE is already covered but map it for ptrace compat */
		gp[HOST_PSTATE] = value;
		break;

	default:
		panic("Bad register in putreg(): %d\n", regno);
	}
	return 0;
}

unsigned long getreg(struct task_struct *child, int regno)
{
	unsigned long *gp = child->thread.regs.regs.gp;

	switch (regno) {
	case 0 ... 33:
		return gp[regno];

	case 34:
		return UPT_SYSCALL_NR(&child->thread.regs.regs);

	case 35:
		return gp[HOST_PSTATE];

	default:
		panic("Bad register in getreg: %d\n", regno);
	}
	return 0;
}

int peek_user(struct task_struct *child, long addr, long data)
{
	unsigned long tmp;

	if ((addr & 7) || addr < 0)
		return -EIO;

	tmp = 0;
	if (addr < MAX_REG_OFFSET)
		tmp = getreg(child, addr >> 3);

	return put_user(tmp, (unsigned long __user *)data);
}

int poke_user(struct task_struct *child, long addr, long data)
{
	if ((addr & 7) || addr < 0)
		return -EIO;

	if (addr < MAX_REG_OFFSET)
		return putreg(child, addr >> 3, data);

	return -EIO;
}

long subarch_ptrace(struct task_struct *child, long request,
		    unsigned long addr, unsigned long data)
{
	int ret = -EIO;
	void __user *datap = (void __user *)data;

	switch (request) {
	case PTRACE_GETREGSET:
	case PTRACE_SETREGSET:
		/* Handled by the generic UML ptrace layer */
		break;
	default:
		break;
	}

	return ret;
}

#include <linux/elf.h>
#include <linux/regset.h>

unsigned long elf_hwcap;

enum arm64_um_regset {
	REGSET_GPR,
};

static int gpr_get(struct task_struct *target,
		   const struct user_regset *regset,
		   struct membuf to)
{
	struct uml_pt_regs *regs = &task_pt_regs(target)->regs;

	return membuf_write(&to, regs->gp, ELF_NGREG * sizeof(elf_greg_t));
}

static int gpr_set(struct task_struct *target,
		   const struct user_regset *regset,
		   unsigned int pos, unsigned int count,
		   const void *kbuf, const void __user *ubuf)
{
	struct uml_pt_regs *regs = &task_pt_regs(target)->regs;

	return user_regset_copyin(&pos, &count, &kbuf, &ubuf, regs->gp,
				  0, ELF_NGREG * sizeof(elf_greg_t));
}

static const struct user_regset arm64_um_regsets[] = {
	[REGSET_GPR] = {
		USER_REGSET_NOTE_TYPE(PRSTATUS),
		.n		= ELF_NGREG,
		.size		= sizeof(elf_greg_t),
		.align		= sizeof(elf_greg_t),
		.regset_get	= gpr_get,
		.set		= gpr_set,
	},
};

static const struct user_regset_view arm64_um_regset_view = {
	.name = "aarch64",
	.e_machine = EM_AARCH64,
	.regsets = arm64_um_regsets,
	.n = ARRAY_SIZE(arm64_um_regsets),
};

const struct user_regset_view *task_user_regset_view(struct task_struct *task)
{
	return &arm64_um_regset_view;
}

int arch_set_tls(struct task_struct *new, unsigned long tls)
{
	new->thread.arch.tpidr_el0 = tls;
	new->thread.regs.regs.tpidr_el0 = tls;
	return 0;
}

void clear_flushed_tls(struct task_struct *task)
{
}

void arch_flush_thread(struct arch_thread *thread)
{
	thread->tpidr_el0 = 0;
	current->thread.regs.regs.tpidr_el0 = 0;
}
