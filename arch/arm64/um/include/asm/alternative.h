/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_ALTERNATIVE_ARM64_H
#define __ASM_UM_ALTERNATIVE_ARM64_H

#include <linux/types.h>

/* UML doesn't need code alternatives — provide minimal stub */
struct alt_instr {
	s32 instr_offset;
	s32 repl_offset;
	u16 cpucap;
	u8  instrlen;
	u8  replacementlen;
};

#endif
