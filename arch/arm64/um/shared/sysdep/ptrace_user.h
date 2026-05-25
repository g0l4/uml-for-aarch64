/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SYSDEP_ARM64_PTRACE_USER_H
#define __SYSDEP_ARM64_PTRACE_USER_H

#include <generated/user_constants.h>

#define PT_OFFSET(r)	((r) * sizeof(long))

#define PT_SYSCALL_NR(regs)	((regs)[HOST_X8])
#define PT_SYSCALL_NR_OFFSET	PT_OFFSET(HOST_X8)

#define PT_SYSCALL_RET_OFFSET	PT_OFFSET(HOST_X0)

#define REGS_IP_INDEX		HOST_PC
#define REGS_SP_INDEX		HOST_SP

#endif
