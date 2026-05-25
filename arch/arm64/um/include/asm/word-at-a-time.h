/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_WORD_AT_A_TIME_ARM64_H
#define __ASM_UM_WORD_AT_A_TIME_ARM64_H

#include <asm-generic/word-at-a-time.h>

/* load_unaligned_zeropad: UML can use a plain memcpy */
static inline unsigned long load_unaligned_zeropad(const void *addr)
{
	unsigned long ret;
	__builtin_memcpy(&ret, addr, sizeof(ret));
	return ret;
}

#endif
