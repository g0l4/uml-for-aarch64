/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_UNWIND_USER_ARM64_H
#define __ASM_UM_UNWIND_USER_ARM64_H

struct unwind_stacktrace;

static inline int unwind_user(struct unwind_stacktrace *trace, unsigned int flags)
{
	return 0;
}

#endif
