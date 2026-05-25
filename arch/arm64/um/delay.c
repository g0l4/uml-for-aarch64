// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <asm-generic/delay.h>

void __delay(unsigned long loops)
{
	/*
	 * Busy-wait loop for very short delays.
	 * UML runs as a host process; use the arch-provided yield.
	 */
	asm volatile("yield" ::: "memory");
}
EXPORT_SYMBOL(__delay);

void __udelay(unsigned long usecs)
{
	/*
	 * On UML, delay via the host OS scheduling.
	 * The UML time subsystem provides the actual delay mechanism.
	 */
	unsigned long loops = usecs * 10;
	__delay(loops);
}
EXPORT_SYMBOL(__udelay);

void __const_udelay(unsigned long xloops)
{
	__delay(xloops);
}
EXPORT_SYMBOL(__const_udelay);
