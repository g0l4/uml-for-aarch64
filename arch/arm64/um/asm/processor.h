/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __UM_PROCESSOR_ARM64_H
#define __UM_PROCESSOR_ARM64_H

#include <linux/time-internal.h>
#include <sysdep/faultinfo.h>

#define KSTK_EIP(tsk) KSTK_REG(tsk, HOST_PC)
#define KSTK_ESP(tsk) KSTK_REG(tsk, HOST_SP)
#define KSTK_EBP(tsk) KSTK_REG(tsk, HOST_X29)

#define ARCH_IS_STACKGROW(address) \
	(address + 65536 + 32 * sizeof(unsigned long) >= UPT_SP(&current->thread.regs.regs))

struct arch_thread {
	unsigned long debugregs[8];
	int debugregs_seq;
	struct faultinfo faultinfo;
	unsigned long tpidr_el0;
};

#define INIT_ARCH_THREAD { .debugregs = { [0 ... 7] = 0 }, \
			   .debugregs_seq = 0, \
			   .faultinfo = { 0, 0, 0 }, \
			   .tpidr_el0 = 0 }

void arch_flush_thread(struct arch_thread *thread);

static inline void arch_copy_thread(struct arch_thread *from,
				    struct arch_thread *to)
{
	to->tpidr_el0 = from->tpidr_el0;
}

#define STACKSLOTS_PER_LINE 4

/* Arm64 uses yield for cpu_relax (no PAUSE instruction like x86) */
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

#define current_sp() ({ void *sp; asm("mov %0, sp" : "=r" (sp)); sp; })
#define current_bp() ({ unsigned long bp; asm("mov %0, x29" : "=r" (bp)); bp; })

#define task_pt_regs(t) (&(t)->thread.regs)

#include <asm/processor-generic.h>

#endif /* __UM_PROCESSOR_ARM64_H */
