/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_ATOMIC_ARM64_H
#define __ASM_UM_ATOMIC_ARM64_H

#include <linux/types.h>
#include <asm/cmpxchg.h>

#define ATOMIC64_INIT(i)	{ (i) }

#define arch_atomic_read(v) \
	__atomic_load_n(&(v)->counter, __ATOMIC_RELAXED)
#define arch_atomic_set(v, i) \
	__atomic_store_n(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_set_release(v, i) \
	__atomic_store_n(&(v)->counter, (i), __ATOMIC_RELEASE)

#define arch_atomic_add(i, v) \
	((void)__atomic_fetch_add(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic_sub(i, v) \
	((void)__atomic_fetch_sub(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic_and(i, v) \
	((void)__atomic_fetch_and(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic_or(i, v) \
	((void)__atomic_fetch_or(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic_xor(i, v) \
	((void)__atomic_fetch_xor(&(v)->counter, (i), __ATOMIC_RELAXED))

#define arch_atomic_add_return_relaxed(i, v) \
	__atomic_add_fetch(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_sub_return_relaxed(i, v) \
	__atomic_sub_fetch(&(v)->counter, (i), __ATOMIC_RELAXED)

#define arch_atomic_fetch_add_relaxed(i, v) \
	__atomic_fetch_add(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_fetch_sub_relaxed(i, v) \
	__atomic_fetch_sub(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_fetch_and_relaxed(i, v) \
	__atomic_fetch_and(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_fetch_or_relaxed(i, v) \
	__atomic_fetch_or(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic_fetch_xor_relaxed(i, v) \
	__atomic_fetch_xor(&(v)->counter, (i), __ATOMIC_RELAXED)

#define arch_atomic_xchg_relaxed(v, i) \
	arch_xchg_relaxed(&(v)->counter, (i))
#define arch_atomic_cmpxchg_relaxed(v, o, n) \
	arch_cmpxchg_relaxed(&(v)->counter, (o), (n))

#define arch_atomic64_read(v) \
	__atomic_load_n(&(v)->counter, __ATOMIC_RELAXED)
#define arch_atomic64_set(v, i) \
	__atomic_store_n(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_set_release(v, i) \
	__atomic_store_n(&(v)->counter, (i), __ATOMIC_RELEASE)

#define arch_atomic64_add(i, v) \
	((void)__atomic_fetch_add(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic64_sub(i, v) \
	((void)__atomic_fetch_sub(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic64_and(i, v) \
	((void)__atomic_fetch_and(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic64_or(i, v) \
	((void)__atomic_fetch_or(&(v)->counter, (i), __ATOMIC_RELAXED))
#define arch_atomic64_xor(i, v) \
	((void)__atomic_fetch_xor(&(v)->counter, (i), __ATOMIC_RELAXED))

#define arch_atomic64_add_return_relaxed(i, v) \
	__atomic_add_fetch(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_sub_return_relaxed(i, v) \
	__atomic_sub_fetch(&(v)->counter, (i), __ATOMIC_RELAXED)

#define arch_atomic64_fetch_add_relaxed(i, v) \
	__atomic_fetch_add(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_fetch_sub_relaxed(i, v) \
	__atomic_fetch_sub(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_fetch_and_relaxed(i, v) \
	__atomic_fetch_and(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_fetch_or_relaxed(i, v) \
	__atomic_fetch_or(&(v)->counter, (i), __ATOMIC_RELAXED)
#define arch_atomic64_fetch_xor_relaxed(i, v) \
	__atomic_fetch_xor(&(v)->counter, (i), __ATOMIC_RELAXED)

#define arch_atomic64_xchg_relaxed(v, i) \
	arch_xchg_relaxed(&(v)->counter, (i))
#define arch_atomic64_cmpxchg_relaxed(v, o, n) \
	arch_cmpxchg_relaxed(&(v)->counter, (o), (n))

#endif
