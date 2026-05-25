/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SYS_SIGCONTEXT_ARM64_H
#define __SYS_SIGCONTEXT_ARM64_H

#include <stub-data.h>

extern void get_regs_from_mc(struct uml_pt_regs *, mcontext_t *);
extern void get_mc_from_regs(struct uml_pt_regs *regs, mcontext_t *mc,
			     int single_stepping);

extern int get_stub_state(struct uml_pt_regs *regs, struct stub_data *data,
			  unsigned long *fp_size_out);
extern int set_stub_state(struct uml_pt_regs *regs, struct stub_data *data,
			  int single_stepping);

/*
 * Extract fault info from the kernel sigcontext mcontext_t.
 * On arm64, the kernel delivers siginfo with si_addr and a separate
 * sigcontext. The stub captures both; here we plumb the fields we need.
 */
#define GET_FAULTINFO_FROM_MC(fi, mc)				\
	{							\
		(fi).addr	= (mc)->fault_address;		\
		(fi).error_code	= (mc)->__reserved[0];		\
		(fi).trap_no	= (mc)->__reserved[1];		\
	}

#endif
