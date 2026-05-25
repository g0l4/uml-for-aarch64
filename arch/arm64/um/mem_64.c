// SPDX-License-Identifier: GPL-2.0
#include <linux/bug.h>
#include <linux/string.h>

/*
 * The arm64 user_regs_struct layout (31 GPRs + SP + PC + PSTATE = 34 quads)
 * is verified at build time by the user-offsets generation.
 */
