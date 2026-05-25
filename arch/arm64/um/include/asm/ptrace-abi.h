/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_PTRACE_ABI_ARM64_H
#define __ASM_UM_PTRACE_ABI_ARM64_H

/*
 * Arm64 ptrace ABI definitions for UML.
 * The register indices are offsets into user_pt_regs / 8.
 */
#define REG_X0		0
#define REG_X1		8
#define REG_X2		16
#define REG_X3		24
#define REG_X4		32
#define REG_X5		40
#define REG_X6		48
#define REG_X7		56
#define REG_X8		64
#define REG_X9		72
#define REG_X10		80
#define REG_X11		88
#define REG_X12		96
#define REG_X13		104
#define REG_X14		112
#define REG_X15		120
#define REG_X16		128
#define REG_X17		136
#define REG_X18		144
#define REG_X19		152
#define REG_X20		160
#define REG_X21		168
#define REG_X22		176
#define REG_X23		184
#define REG_X24		192
#define REG_X25		200
#define REG_X26		208
#define REG_X27		216
#define REG_X28		224
#define REG_X29		232
#define REG_X30		240
#define REG_SP		248
#define REG_PC		256
#define REG_PSTATE	264

#define REG_X0_IDX	(REG_X0 / 8)
#define REG_SP_IDX	(REG_SP / 8)
#define REG_PC_IDX	(REG_PC / 8)
#define REG_PSTATE_IDX	(REG_PSTATE / 8)

#endif
