/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_ELF_ARM64_H
#define __ASM_UM_ELF_ARM64_H

#include <asm/elf.h>

#define CORE_DUMP_USE_REGSET

#define ELF_CORE_EFLAGS 0

#define ELF_EXEC_PAGESIZE 4096
#define ELF_ET_DYN_BASE (2 * TASK_SIZE / 3)

/*
 * ELF register definitions.
 */
#define ELF_NGREG 34	/* 31 GPRs + SP + PC + PSTATE */
typedef unsigned long elf_greg_t;
typedef elf_greg_t elf_gregset_t[ELF_NGREG];

#define ELF_CORE_COPY_REGS(pr_reg, regs)				\
	do {								\
		int __i;						\
									\
		for (__i = 0; __i < ELF_NGREG; __i++)			\
			(pr_reg)[__i] = (regs)->regs.gp[__i];		\
	} while (0);

#define ELF_NFPREG 34	/* 32 V-regs + FPSR + FPCR, each 16 bytes, but count as 34 longs for compat */
typedef unsigned long elf_fpreg_t;
typedef elf_fpreg_t elf_fpregset_t[ELF_NFPREG];

/*
 * These are used to set parameters in the core dumps.
 */
#define ELF_CLASS	ELFCLASS64
#define ELF_DATA	ELFDATA2LSB
#define ELF_ARCH	EM_AARCH64

#define ELF_PLATFORM	"aarch64"

#define SET_PERSONALITY(ex) set_personality(PER_LINUX)

#define ELF_HWCAP	(elf_hwcap)

extern unsigned long elf_hwcap;

/*
 * elf_check_arch: validate the ELF header for this architecture.
 * Always accept native aarch64 ELF executables.
 */
#define elf_check_arch(x) \
	((x)->e_machine == EM_AARCH64)

#endif
