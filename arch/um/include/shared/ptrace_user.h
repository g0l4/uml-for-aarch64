/* SPDX-License-Identifier: GPL-2.0 */
/* 
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 */

#ifndef __PTRACE_USER_H__
#define __PTRACE_USER_H__

#include <sys/ptrace.h>
#include <sysdep/ptrace_user.h>

struct uml_pt_regs;

extern int ptrace_getregs(long pid, unsigned long *regs_out);
extern int ptrace_setregs(long pid, unsigned long *regs_in);
extern int ptrace_get_thread_state(long pid, struct uml_pt_regs *regs);
extern int ptrace_set_thread_state(long pid, struct uml_pt_regs *regs);
extern int ptrace_get_syscall_nr(long pid, long *nr);
extern int ptrace_set_syscall_nr(long pid, long nr);
extern int ptrace_set_syscall_ret(long pid, unsigned long ret);

#endif
