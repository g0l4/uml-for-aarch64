/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_SPINLOCK_ARM64_H
#define __ASM_UM_SPINLOCK_ARM64_H

#include <asm/qspinlock.h>
#include <asm/qrwlock.h>

#define smp_mb__after_spinlock()	smp_mb()

#define vcpu_is_preempted vcpu_is_preempted
static inline bool vcpu_is_preempted(int cpu)
{
	return false;
}

#endif
