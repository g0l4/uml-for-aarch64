// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <asm-generic/delay.h>

void __delay(unsigned long loops)
{
	/*
	 * Busy-wait loop — UML runs as a host process, so we spin
	 * rather than reading a hardware cycle counter.
	 */
	asm volatile(
		"1: subs %0, %0, #1\n"
		"   b.ne 1b\n"
		: "+r" (loops)
		:
		: "cc", "memory"
	);
}
EXPORT_SYMBOL(__delay);

void __udelay(unsigned long usecs)
{
	/*
	 * Approximate microsecond delay: scale by a rough estimate
	 * of loop iterations per microsecond.  The BogoMIPS calibration
	 * will refine this at boot.
	 */
	unsigned long loops = usecs * 100;
	__delay(loops);
}
EXPORT_SYMBOL(__udelay);

void __const_udelay(unsigned long xloops)
{
	__delay(xloops);
}
EXPORT_SYMBOL(__const_udelay);
