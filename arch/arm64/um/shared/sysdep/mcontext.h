/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SYS_SIGCONTEXT_ARM64_H
#define __SYS_SIGCONTEXT_ARM64_H

#include <sys/ucontext.h>
#include <stub-data.h>
#include <sysdep/faultinfo.h>

#ifndef FPSIMD_MAGIC
#define FPSIMD_MAGIC	0x46508001
#endif

#ifndef ESR_MAGIC
#define ESR_MAGIC	0x45535201
#endif

struct uml_aarch64_ctx {
	unsigned int magic;
	unsigned int size;
};

struct uml_aarch64_esr_context {
	struct uml_aarch64_ctx head;
	unsigned long esr;
};

struct uml_aarch64_fpsimd_context {
	struct uml_aarch64_ctx head;
	unsigned int fpsr;
	unsigned int fpcr;
	__uint128_t vregs[32];
};

extern void get_regs_from_mc(struct uml_pt_regs *, mcontext_t *);
extern void get_mc_from_regs(struct uml_pt_regs *regs, mcontext_t *mc,
			     int single_stepping);

extern int get_stub_state(struct uml_pt_regs *regs, struct stub_data *data,
			  unsigned long *fp_size_out);
extern int set_stub_state(struct uml_pt_regs *regs, struct stub_data *data,
			  int single_stepping);

static inline unsigned long get_esr_from_mc(mcontext_t *mc)
{
	struct uml_aarch64_ctx *ctx = (struct uml_aarch64_ctx *)mc->__reserved;
	unsigned long offset = 0;

	while (offset + sizeof(*ctx) <= sizeof(mc->__reserved)) {
		if (ctx->magic == 0 || ctx->size == 0)
			break;
		if (ctx->size < sizeof(*ctx) ||
		    ctx->size > sizeof(mc->__reserved) - offset)
			break;
		if (ctx->magic == ESR_MAGIC &&
		    ctx->size >= sizeof(struct uml_aarch64_esr_context))
			return ((struct uml_aarch64_esr_context *)ctx)->esr;

		offset += ctx->size;
		ctx = (struct uml_aarch64_ctx *)((char *)ctx + ctx->size);
	}

	return 0;
}

#define GET_FAULTINFO_FROM_MC(fi, mc)					\
	do {								\
		unsigned long esr = get_esr_from_mc(mc);		\
									\
		(fi).addr = (mc)->fault_address;			\
		(fi).error_code = esr;					\
		(fi).trap_no = ARM64_ESR_EC(esr);			\
	} while (0)

#endif
