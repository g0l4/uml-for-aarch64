// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <signal.h>
#include <poll.h>
#include <sys/mman.h>
#include <sys/user.h>
#define __FRAME_OFFSETS
#include <linux/ptrace.h>
#include <asm/types.h>
#include <linux/kbuild.h>

#define DEFINE_LONGS(sym, val) \
	COMMENT(#val " / sizeof(unsigned long)"); \
	DEFINE(sym, val / sizeof(unsigned long))

void foo(void);

void foo(void)
{
	/*
	 * Arm64 user_regs_struct layout:
	 *   unsigned long long regs[31];
	 *   unsigned long long sp;
	 *   unsigned long long pc;
	 *   unsigned long long pstate;
	 */

	/* GPRs — offset directly as index */
	DEFINE_LONGS(HOST_X0, 0);
	DEFINE_LONGS(HOST_X1, 8);
	DEFINE_LONGS(HOST_X2, 16);
	DEFINE_LONGS(HOST_X3, 24);
	DEFINE_LONGS(HOST_X4, 32);
	DEFINE_LONGS(HOST_X5, 40);
	DEFINE_LONGS(HOST_X6, 48);
	DEFINE_LONGS(HOST_X7, 56);
	DEFINE_LONGS(HOST_X8, 64);
	DEFINE_LONGS(HOST_X9, 72);
	DEFINE_LONGS(HOST_X10, 80);
	DEFINE_LONGS(HOST_X11, 88);
	DEFINE_LONGS(HOST_X12, 96);
	DEFINE_LONGS(HOST_X13, 104);
	DEFINE_LONGS(HOST_X14, 112);
	DEFINE_LONGS(HOST_X15, 120);
	DEFINE_LONGS(HOST_X16, 128);
	DEFINE_LONGS(HOST_X17, 136);
	DEFINE_LONGS(HOST_X18, 144);
	DEFINE_LONGS(HOST_X19, 152);
	DEFINE_LONGS(HOST_X20, 160);
	DEFINE_LONGS(HOST_X21, 168);
	DEFINE_LONGS(HOST_X22, 176);
	DEFINE_LONGS(HOST_X23, 184);
	DEFINE_LONGS(HOST_X24, 192);
	DEFINE_LONGS(HOST_X25, 200);
	DEFINE_LONGS(HOST_X26, 208);
	DEFINE_LONGS(HOST_X27, 216);
	DEFINE_LONGS(HOST_X28, 224);
	DEFINE_LONGS(HOST_X29, 232);
	DEFINE_LONGS(HOST_X30, 240);

	/* Special registers */
	DEFINE_LONGS(HOST_SP, 248);
	DEFINE_LONGS(HOST_PC, 256);
	DEFINE_LONGS(HOST_PSTATE, 264);

	DEFINE(UM_FRAME_SIZE, sizeof(struct user_regs_struct));

	DEFINE(UM_POLLIN, POLLIN);
	DEFINE(UM_POLLPRI, POLLPRI);
	DEFINE(UM_POLLOUT, POLLOUT);

	DEFINE(UM_PROT_READ, PROT_READ);
	DEFINE(UM_PROT_WRITE, PROT_WRITE);
	DEFINE(UM_PROT_EXEC, PROT_EXEC);
}
