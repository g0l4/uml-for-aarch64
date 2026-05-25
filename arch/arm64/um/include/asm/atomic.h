/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_ATOMIC_ARM64_H
#define __ASM_UM_ATOMIC_ARM64_H

/*
 * UML-on-arm64 atomics using compiler __atomic builtins.
 * All arch_atomic_* / arch_atomic64_* are macros so the
 * preprocessor-based fallback framework can detect them.
 */

#include <linux/compiler.h>
#include <linux/types.h>
#include <asm/cmpxchg.h>

#define arch_atomic_read(v)		__atomic_load_n(&(v)->counter, __ATOMIC_RELAXED)
#define arch_atomic_set(v, i)		__atomic_store_n(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_add(i, v)		((void)__atomic_fetch_add(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic_sub(i, v)		((void)__atomic_fetch_sub(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic_and(i, v)		((void)__atomic_fetch_and(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic_or(i, v)		((void)__atomic_fetch_or(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic_xor(i, v)		((void)__atomic_fetch_xor(&(v)->counter, (i), __ATOMIC_RELAXED))

#define arch_atomic_add_return(i, v)	__atomic_add_fetch(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_sub_return(i, v)	__atomic_sub_fetch(&(v)->counter, (i), __ATOMIC_RELAXED)

#define arch_atomic_fetch_add(i, v)	__atomic_fetch_add(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_fetch_sub(i, v)	__atomic_fetch_sub(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_fetch_and(i, v)	__atomic_fetch_and(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_fetch_or(i, v)	__atomic_fetch_or(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_fetch_xor(i, v)	__atomic_fetch_xor(&(v)->counter, (i), __ATOMIC_RELAXED)

/* 64-bit operations */
#define arch_atomic64_read(v)		__atomic_load_n(&(v)->counter, __ATOMIC_RELAXED)
#define arch_atomic64_set(v, i)		__atomic_store_n(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_add(i, v)		((void)__atomic_fetch_add(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic64_sub(i, v)		((void)__atomic_fetch_sub(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic64_and(i, v)		((void)__atomic_fetch_and(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic64_or(i, v)		((void)__atomic_fetch_or(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic64_xor(i, v)		((void)__atomic_fetch_xor(&(v)->counter, (i), __ATOMIC_RELAXED))

#define arch_atomic64_add_return(i, v)	__atomic_add_fetch(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_sub_return(i, v)	__atomic_sub_fetch(&(v)->counter, (i), __ATOMIC_RELAXED)

#define arch_atomic64_fetch_add(i, v)	__atomic_fetch_add(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_fetch_sub(i, v)	__atomic_fetch_sub(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_fetch_and(i, v)	__atomic_fetch_and(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_fetch_or(i, v)	__atomic_fetch_or(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_fetch_xor(i, v)	__atomic_fetch_xor(&(v)->counter, (i), __ATOMIC_RELAXED)

#define ATOMIC64_INIT(i)	{ (i) }

#endif
