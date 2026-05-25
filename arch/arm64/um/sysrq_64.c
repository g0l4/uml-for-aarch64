// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <registers.h>
#include <sysdep/ptrace.h>

void show_regs(struct pt_regs *regs)
{
	int i;

	printk(KERN_INFO "UML arm64 registers:\n");
	printk(KERN_INFO "  PC:  %016lx  SP:  %016lx  PSTATE: %016lx\n",
	       UPT_IP(&regs->regs), UPT_SP(&regs->regs),
	       UPT_PSTATE(&regs->regs));

	for (i = 0; i < 30; i += 2)
		printk(KERN_INFO "  x%-2d: %016lx  x%-2d: %016lx\n",
		       i, regs->regs.gp[i], i + 1, regs->regs.gp[i + 1]);

	printk(KERN_INFO "  x30: %016lx\n", regs->regs.gp[30]);
}
