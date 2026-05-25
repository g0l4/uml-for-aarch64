/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __UM_ARM64_PTRACE_H
#define __UM_ARM64_PTRACE_H

#include <linux/compiler.h>
#include <asm/ptrace-compat.h>
#define __FRAME_OFFSETS
#include <asm/ptrace-generic.h>

#define user_mode(r) UPT_IS_USER(&(r)->regs)

/*
 * PT_REGS_* aliases — the generic code uses the x86 register names.
 * Map them to arm64 registers using the A64 PCS convention.
 */
#define PT_REGS_AX(r)		UPT_X0(&(r)->regs)
#define PT_REGS_DI(r)		UPT_X1(&(r)->regs)
#define PT_REGS_SI(r)		UPT_X2(&(r)->regs)
#define PT_REGS_DX(r)		UPT_X3(&(r)->regs)
#define PT_REGS_CX(r)		UPT_X4(&(r)->regs)
#define PT_REGS_BX(r)		UPT_X5(&(r)->regs)
#define PT_REGS_BP(r)		UPT_X29(&(r)->regs)
#define PT_REGS_EFLAGS(r)	UPT_PSTATE(&(r)->regs)

/* No segment registers on arm64 — return 0 silently */
#define PT_REGS_CS(r)		({ (void)(r); 0UL; })
#define PT_REGS_SS(r)		({ (void)(r); 0UL; })
#define PT_REGS_DS(r)		({ (void)(r); 0UL; })
#define PT_REGS_ES(r)		({ (void)(r); 0UL; })

#define PT_REGS_ORIG_SYSCALL(r) UPT_X8(&(r)->regs)
#define PT_REGS_SYSCALL_RET(r)	UPT_X0(&(r)->regs)

#define PT_FIX_EXEC_STACK(sp) do { } while (0)

#define profile_pc(regs) PT_REGS_IP(regs)

/* On arm64, signal restarts use a special trampoline */
#define UPT_RESTART_SYSCALL(r)	(UPT_SYSCALL_NR(r))

#define PT_REGS_SET_SYSCALL_RETURN(r, res) (UPT_X0(&(r)->regs) = (res))

static inline long regs_return_value(struct pt_regs *regs)
{
	return UPT_X0(&regs->regs);
}

#define user_stack_pointer(regs) PT_REGS_SP(regs)

extern void arch_switch_to(struct task_struct *to);

#endif /* __UM_ARM64_PTRACE_H */
