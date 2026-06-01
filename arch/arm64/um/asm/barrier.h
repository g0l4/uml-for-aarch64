/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_UM_BARRIER_ARM64_H_
#define _ASM_UM_BARRIER_ARM64_H_

/*
 * Memory barriers for UML-on-arm64.
 * Since UML runs as a userspace process on a real CPU, we use the
 * native arm64 barrier instructions.
 */

#define mb()	asm volatile("dmb ish" : : : "memory")
#define rmb()	asm volatile("dmb ishld" : : : "memory")
#define wmb()	asm volatile("dmb ishst" : : : "memory")

#define dma_mb()	asm volatile("dmb osh" : : : "memory")
#define dma_rmb()	asm volatile("dmb oshld" : : : "memory")
#define dma_wmb()	asm volatile("dmb oshst" : : : "memory")

#ifndef __ASSEMBLY__
#include <shared/timetravel.h>

void time_travel_ndelay(unsigned long nsec);

#ifndef cpu_relax
#define cpu_relax cpu_relax

static __always_inline void native_pause(void)
{
	asm volatile("yield" ::: "memory");
}

static __always_inline void cpu_relax(void)
{
	if (time_travel_mode == TT_MODE_INFCPU ||
	    time_travel_mode == TT_MODE_EXTERNAL)
		time_travel_ndelay(1);
	else
		native_pause();
}
#endif
#endif

#include <asm-generic/barrier.h>

#endif
