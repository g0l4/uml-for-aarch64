// SPDX-License-Identifier: GPL-2.0
#define __FRAME_OFFSETS
#include <linux/errno.h>
#include <linux/string.h>
#include <sys/ucontext.h>
#include <asm/ptrace.h>
#include <sysdep/ptrace.h>
#include <sysdep/mcontext.h>
#include <arch.h>

/*
 * Arm64 mcontext_t is defined by <sys/ucontext.h> and contains the
 * full sigcontext (regs[], sp, pc, pstate, fault_address, __reserved[]).
 */
void get_regs_from_mc(struct uml_pt_regs *regs, mcontext_t *mc)
{
	int i;

	for (i = 0; i < 31; i++)
		regs->gp[i] = mc->regs[i];

	regs->gp[HOST_SP] = mc->sp;
	regs->gp[HOST_PC] = mc->pc;
	regs->gp[HOST_PSTATE] = mc->pstate;
}

void mc_set_rip(void *_mc, void *target)
{
	mcontext_t *mc = _mc;

	mc->pc = (unsigned long)target;
}

void get_mc_from_regs(struct uml_pt_regs *regs, mcontext_t *mc,
		      int single_stepping)
{
	int i;

	for (i = 0; i < 31; i++)
		mc->regs[i] = regs->gp[i];

	mc->sp = regs->gp[HOST_SP];
	mc->pc = regs->gp[HOST_PC];
	mc->pstate = regs->gp[HOST_PSTATE];

	/*
	 * Arm64 single-step is managed by the debug hardware, not PSTATE.
	 * UML handles stepping at a higher level, so this is a no-op.
	 */
}

/*
 * Parse the fpsimd_context from the sigcontext __reserved[] area
 * to determine the FP state size. Returns NULL if no valid state found.
 */
static struct uml_aarch64_fpsimd_context *get_fpstate(struct stub_data *data,
						      mcontext_t *mcontext,
						      int *fp_size)
{
	struct uml_aarch64_ctx *ctx;
	unsigned long offset = 0;

	ctx = (struct uml_aarch64_ctx *)mcontext->__reserved;

	while (offset < sizeof(mcontext->__reserved)) {
		if (ctx->magic == 0 || ctx->size == 0)
			break;

		if (ctx->magic == FPSIMD_MAGIC &&
		    ctx->size >= sizeof(struct uml_aarch64_fpsimd_context)) {
			*fp_size = ctx->size;
			return (struct uml_aarch64_fpsimd_context *)ctx;
		}

		if (ctx->size < sizeof(*ctx))
			break;

		offset += ctx->size;
		ctx = (struct uml_aarch64_ctx *)((char *)ctx + ctx->size);
	}

	/* Default: no FP context */
	*fp_size = 0;
	return NULL;
}

int get_stub_state(struct uml_pt_regs *regs, struct stub_data *data,
		   unsigned long *fp_size_out)
{
	mcontext_t *mcontext;
	struct uml_aarch64_fpsimd_context *fpstate_stub;
	int fp_size;

	mcontext = (void *)&data->sigstack[data->mctx_offset];

	get_regs_from_mc(regs, mcontext);
	regs->tpidr_el0 = data->arch_data.tpidr_el0;

	fpstate_stub = get_fpstate(data, mcontext, &fp_size);
	if (!fpstate_stub) {
		if (fp_size_out)
			*fp_size_out = 0;
		return 0;
	}

	if (fp_size_out)
		*fp_size_out = fp_size;

	if (fp_size > host_fp_size)
		return -ENOSPC;

	memcpy(&regs->fp, fpstate_stub, fp_size);

	return 0;
}

int set_stub_state(struct uml_pt_regs *regs, struct stub_data *data,
		   int single_stepping)
{
	mcontext_t *mcontext;
	struct uml_aarch64_fpsimd_context *fpstate_stub;
	int fp_size;

	mcontext = (void *)&data->sigstack[data->mctx_offset];

	if ((unsigned long)mcontext < (unsigned long)data->sigstack ||
	    (unsigned long)mcontext >
			(unsigned long)data->sigstack +
			sizeof(data->sigstack) - sizeof(*mcontext))
		return -EINVAL;

	get_mc_from_regs(regs, mcontext, single_stepping);

	if (data->arch_data.tpidr_el0 != regs->tpidr_el0) {
		data->arch_data.tpidr_el0 = regs->tpidr_el0;
		data->arch_data.sync |= STUB_SYNC_TPIDR;
	}

	fpstate_stub = get_fpstate(data, mcontext, &fp_size);
	if (!fpstate_stub)
		return 0;

	memcpy(fpstate_stub, &regs->fp, fp_size);

	return 0;
}
