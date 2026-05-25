/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_CMPXCHG_ARM64_H
#define __ASM_UM_CMPXCHG_ARM64_H

/*
 * UML on arm64: xchg/cmpxchg using compiler builtins.
 */

#include <linux/compiler.h>
#include <linux/types.h>

/* Include generic first so _local and friends are available */
#include <asm-generic/cmpxchg.h>

/* Override arch_xchg and arch_cmpxchg with builtin-based versions */
#undef arch_xchg
#define arch_xchg(ptr, x)						\
({									\
	typeof(*(ptr)) __x = (x);					\
	__atomic_exchange_n((ptr), __x, __ATOMIC_RELAXED);		\
})

#undef arch_cmpxchg
#define arch_cmpxchg(ptr, o, n)						\
({									\
	typeof(*(ptr)) __o = (o);					\
	__atomic_compare_exchange_n((ptr), &__o, (n), 0,		\
		__ATOMIC_RELAXED, __ATOMIC_RELAXED);			\
	__o;								\
})

#endif
