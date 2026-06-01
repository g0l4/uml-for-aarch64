/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_CMPXCHG_ARM64_H
#define __ASM_UM_CMPXCHG_ARM64_H

#include <linux/build_bug.h>
#include <linux/compiler.h>
#include <linux/types.h>
#include <asm-generic/cmpxchg-local.h>

#define __UM_XCHG_CASE(size, ptr, old, new, mo)				\
	case size:							\
		(old) = (__typeof__(*(ptr)))__atomic_exchange_n(	\
			(__typeof__(*(ptr)) *)(ptr), (new), (mo));	\
		break

#define __um_xchg(ptr, x, mo)						\
({									\
	__typeof__(*(ptr)) __ret;					\
	__typeof__(*(ptr)) __x = (x);					\
									\
	switch (sizeof(*(ptr))) {					\
	__UM_XCHG_CASE(1, (ptr), __ret, __x, (mo));			\
	__UM_XCHG_CASE(2, (ptr), __ret, __x, (mo));			\
	__UM_XCHG_CASE(4, (ptr), __ret, __x, (mo));			\
	__UM_XCHG_CASE(8, (ptr), __ret, __x, (mo));			\
	default:							\
		BUILD_BUG();						\
	}								\
									\
	__ret;								\
})

#define __UM_CMPXCHG_CASE(size, ptr, old, new, ret, smo, fmo)		\
	case size:							\
		(ret) = (old);						\
		__atomic_compare_exchange_n(				\
			(__typeof__(*(ptr)) *)(ptr), &(ret), (new),	\
			0, (smo), (fmo));				\
		break

#define __um_cmpxchg(ptr, old, new, smo, fmo)				\
({									\
	__typeof__(*(ptr)) __old = (old);				\
	__typeof__(*(ptr)) __new = (new);				\
	__typeof__(*(ptr)) __ret;					\
									\
	switch (sizeof(*(ptr))) {					\
	__UM_CMPXCHG_CASE(1, (ptr), __old, __new, __ret, (smo), (fmo));	\
	__UM_CMPXCHG_CASE(2, (ptr), __old, __new, __ret, (smo), (fmo));	\
	__UM_CMPXCHG_CASE(4, (ptr), __old, __new, __ret, (smo), (fmo));	\
	__UM_CMPXCHG_CASE(8, (ptr), __old, __new, __ret, (smo), (fmo));	\
	default:							\
		BUILD_BUG();						\
	}								\
									\
	__ret;								\
})

#define arch_xchg_relaxed(ptr, x)	__um_xchg((ptr), (x), __ATOMIC_RELAXED)
#define arch_xchg_acquire(ptr, x)	__um_xchg((ptr), (x), __ATOMIC_ACQUIRE)
#define arch_xchg_release(ptr, x)	__um_xchg((ptr), (x), __ATOMIC_RELEASE)
#define arch_xchg(ptr, x)		__um_xchg((ptr), (x), __ATOMIC_SEQ_CST)

#define arch_cmpxchg_relaxed(ptr, o, n) \
	__um_cmpxchg((ptr), (o), (n), __ATOMIC_RELAXED, __ATOMIC_RELAXED)
#define arch_cmpxchg_acquire(ptr, o, n) \
	__um_cmpxchg((ptr), (o), (n), __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)
#define arch_cmpxchg_release(ptr, o, n) \
	__um_cmpxchg((ptr), (o), (n), __ATOMIC_RELEASE, __ATOMIC_RELAXED)
#define arch_cmpxchg(ptr, o, n) \
	__um_cmpxchg((ptr), (o), (n), __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)

#define arch_cmpxchg_local(ptr, o, n) \
	((__typeof__(*(ptr)))__generic_cmpxchg_local((ptr),		\
		(unsigned long)(o), (unsigned long)(n), sizeof(*(ptr))))
#define arch_cmpxchg64_local(ptr, o, n) \
	__generic_cmpxchg64_local((ptr), (o), (n))

#define arch_cmpxchg64_relaxed		arch_cmpxchg_relaxed
#define arch_cmpxchg64_acquire		arch_cmpxchg_acquire
#define arch_cmpxchg64_release		arch_cmpxchg_release
#define arch_cmpxchg64			arch_cmpxchg

#define arch_sync_cmpxchg		arch_cmpxchg

#endif
