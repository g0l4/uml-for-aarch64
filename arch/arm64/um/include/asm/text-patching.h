/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_TEXT_PATCHING_ARM64_H
#define __ASM_UM_TEXT_PATCHING_ARM64_H

/*
 * UML doesn't need runtime code patching.
 */

static inline int text_poke_early(void *addr, const void *opcode, size_t len)
{
	return 0;
}

#endif
