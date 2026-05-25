/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_ARM64_UM_CPUFEATURES_H
#define __ASM_ARM64_UM_CPUFEATURES_H

/*
 * Minimal cpufeature definitions for UML-on-arm64.
 * The UML core uses the x86 cpufeature infrastructure to store boot-time
 * feature bits.  Provide just enough to satisfy the generic code.
 */
#define NCAPINTS	2
#define NBUGINTS	1

/* Feature numbers — only the ones the UML core references. */
#define X86_FEATURE_FPU		(0*32+ 0)
#define X86_FEATURE_CX8		(0*32+ 8)
#define X86_FEATURE_PGE		(0*32+13)
#define X86_FEATURE_CLFLUSH	(0*32+19)
#define X86_FEATURE_MMX		(0*32+23)
#define X86_FEATURE_FXSR	(0*32+24)
#define X86_FEATURE_PAT		(0*32+16)
#define X86_FEATURE_TSC		(0*32+ 4)

#define X86_FEATURE_XMM2	(0*32+26)
#define X86_FEATURE_NX		(0*32+20)
#define X86_FEATURE_SYSCALL	(0*32+11)
#define X86_FEATURE_GBPAGES	(0*32+26)

#define X86_FEATURE_PCID	(1*32+ 5)

#define X86_FEATURE_FSGSBASE	(1*32+11)
#define X86_FEATURE_TSC_ADJUST	(1*32+16)

#define X86_FEATURE_ALWAYS	(2*32+ 0)	/* sentinel */

#define X86_BUG_F00F		(NCAPINTS*32+0)
#define X86_BUG_CPU_MELTDOWN	(NCAPINTS*32+1)
#define X86_BUG_SPECTRE_V1	(NCAPINTS*32+2)
#define X86_BUG_SPECTRE_V2	(NCAPINTS*32+3)

#endif
