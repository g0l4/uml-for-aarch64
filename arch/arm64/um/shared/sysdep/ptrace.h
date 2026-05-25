/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SYSDEP_ARM64_PTRACE_H
#define __SYSDEP_ARM64_PTRACE_H

#include <generated/user_constants.h>
#include <sysdep/faultinfo.h>

#define MAX_REG_OFFSET (UM_FRAME_SIZE)
#define MAX_REG_NR ((MAX_REG_OFFSET) / sizeof(unsigned long))

/* Register access macros — gp[] indices map to user_regs_struct layout */
#define REGS_IP(r)	((r)[HOST_PC])
#define REGS_SP(r)	((r)[HOST_SP])
#define REGS_PSTATE(r)	((r)[HOST_PSTATE])

/* General-purpose registers by A64 calling-convention roles */
#define REGS_X0(r)	((r)[0])
#define REGS_X1(r)	((r)[1])
#define REGS_X2(r)	((r)[2])
#define REGS_X3(r)	((r)[3])
#define REGS_X4(r)	((r)[4])
#define REGS_X5(r)	((r)[5])
#define REGS_X6(r)	((r)[6])
#define REGS_X7(r)	((r)[7])
#define REGS_X8(r)	((r)[8])
#define REGS_X9(r)	((r)[9])
#define REGS_X10(r)	((r)[10])
#define REGS_X11(r)	((r)[11])
#define REGS_X12(r)	((r)[12])
#define REGS_X13(r)	((r)[13])
#define REGS_X14(r)	((r)[14])
#define REGS_X15(r)	((r)[15])
#define REGS_X16(r)	((r)[16])
#define REGS_X17(r)	((r)[17])
#define REGS_X18(r)	((r)[18])
#define REGS_X19(r)	((r)[19])
#define REGS_X20(r)	((r)[20])
#define REGS_X21(r)	((r)[21])
#define REGS_X22(r)	((r)[22])
#define REGS_X23(r)	((r)[23])
#define REGS_X24(r)	((r)[24])
#define REGS_X25(r)	((r)[25])
#define REGS_X26(r)	((r)[26])
#define REGS_X27(r)	((r)[27])
#define REGS_X28(r)	((r)[28])
#define REGS_X29(r)	((r)[29])
#define REGS_X30(r)	((r)[30])

/* Convenience aliases matching the A64 PCS */
#define REGS_FP(r)	REGS_X29(r)
#define REGS_LR(r)	REGS_X30(r)

/* UPT accessors */
#define UPT_IP(r)	REGS_IP((r)->gp)
#define UPT_SP(r)	REGS_SP((r)->gp)
#define UPT_PSTATE(r)	REGS_PSTATE((r)->gp)

/* Syscall convention: NR in x8, return in x0 */
#define UPT_SYSCALL_NR(r)	REGS_X8((r)->gp)
#define UPT_SYSCALL_RET(r)	REGS_X0((r)->gp)

/* Syscall arguments (x0-x5) for restart */
#define UPT_SYSCALL_ARG1(r)	UPT_X0(r)
#define UPT_SYSCALL_ARG2(r)	UPT_X1(r)
#define UPT_SYSCALL_ARG3(r)	UPT_X2(r)
#define UPT_SYSCALL_ARG4(r)	UPT_X3(r)
#define UPT_SYSCALL_ARG5(r)	UPT_X4(r)
#define UPT_SYSCALL_ARG6(r)	UPT_X5(r)

/* Per-register UPT accessors */
#define UPT_X0(r)	REGS_X0((r)->gp)
#define UPT_X1(r)	REGS_X1((r)->gp)
#define UPT_X2(r)	REGS_X2((r)->gp)
#define UPT_X3(r)	REGS_X3((r)->gp)
#define UPT_X4(r)	REGS_X4((r)->gp)
#define UPT_X5(r)	REGS_X5((r)->gp)
#define UPT_X6(r)	REGS_X6((r)->gp)
#define UPT_X7(r)	REGS_X7((r)->gp)
#define UPT_X8(r)	REGS_X8((r)->gp)
#define UPT_X9(r)	REGS_X9((r)->gp)
#define UPT_X10(r)	REGS_X10((r)->gp)
#define UPT_X11(r)	REGS_X11((r)->gp)
#define UPT_X12(r)	REGS_X12((r)->gp)
#define UPT_X13(r)	REGS_X13((r)->gp)
#define UPT_X14(r)	REGS_X14((r)->gp)
#define UPT_X15(r)	REGS_X15((r)->gp)
#define UPT_X16(r)	REGS_X16((r)->gp)
#define UPT_X17(r)	REGS_X17((r)->gp)
#define UPT_X18(r)	REGS_X18((r)->gp)
#define UPT_X19(r)	REGS_X19((r)->gp)
#define UPT_X20(r)	REGS_X20((r)->gp)
#define UPT_X21(r)	REGS_X21((r)->gp)
#define UPT_X22(r)	REGS_X22((r)->gp)
#define UPT_X23(r)	REGS_X23((r)->gp)
#define UPT_X24(r)	REGS_X24((r)->gp)
#define UPT_X25(r)	REGS_X25((r)->gp)
#define UPT_X26(r)	REGS_X26((r)->gp)
#define UPT_X27(r)	REGS_X27((r)->gp)
#define UPT_X28(r)	REGS_X28((r)->gp)
#define UPT_X29(r)	REGS_X29((r)->gp)
#define UPT_X30(r)	REGS_X30((r)->gp)

/* Pointer-auth masks; we don't currently manage PAC keys in UML. */
#define UPT_PAC_IAKEY(r)	({ (void)(r); 0UL; })
#define UPT_PAC_IBKEY(r)	({ (void)(r); 0UL; })
#define UPT_PAC_DAKEY(r)	({ (void)(r); 0UL; })
#define UPT_PAC_DBKEY(r)	({ (void)(r); 0UL; })

extern unsigned long host_fp_size;

struct uml_pt_regs {
	unsigned long gp[MAX_REG_NR];
	struct faultinfo faultinfo;
	long syscall;
	int is_user;

	/* Dynamically sized SIMD/SVE state */
	unsigned long fp[];
};

#define EMPTY_UML_PT_REGS { }

#define UPT_FAULTINFO(r)	(&(r)->faultinfo)
#define UPT_IS_USER(r)		((r)->is_user)

/* UPT_SYSCALL_NR / RET are already defined above */
#define UPT_RESTART_SYSCALL(r)	(UPT_SYSCALL_NR(r))

extern int arch_init_registers(int pid);

/*
 * X86-specific ptrace constants not present on arm64.
 * Provide stub values so UML generic code compiles.
 * Arm64 UML always uses seccomp mode; non-seccomp paths are never taken.
 */
#ifndef PTRACE_GETREGS
#define PTRACE_GETREGS 0x4100
#endif
#ifndef PTRACE_SETREGS
#define PTRACE_SETREGS 0x4200
#endif
#ifndef PTRACE_SYSEMU
#define PTRACE_SYSEMU 0x4300
#endif
#ifndef PTRACE_SYSEMU_SINGLESTEP
#define PTRACE_SYSEMU_SINGLESTEP 0x4400
#endif
#ifndef PTRACE_GET_THREAD_AREA
#define PTRACE_GET_THREAD_AREA 0x4500
#endif
#ifndef PTRACE_SET_THREAD_AREA
#define PTRACE_SET_THREAD_AREA 0x4600
#endif

#endif /* __SYSDEP_ARM64_PTRACE_H */
