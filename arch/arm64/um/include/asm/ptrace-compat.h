/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_PTRACE_COMPAT_ARM64_H
#define __ASM_UM_PTRACE_COMPAT_ARM64_H

/*
 * Arm64 does not have PTRACE_GETREGS/SETREGS/SYSEMU/GET_THREAD_AREA/SET_THREAD_AREA.
 * Define unique stub values so the UML generic code compiles.
 * The non-seccomp path is not supported on arm64 and
 * using_seccomp is always forced to 1.
 */

#ifndef PTRACE_GETREGS
#define PTRACE_GETREGS 0x4100
#endif
#ifndef PTRACE_SETREGS
#define PTRACE_SETREGS 0x4200
#endif
#ifndef PTRACE_SYSEMU
#define PTRACE_SYSEMU 0x4300
#endif
#ifndef PTRACE_SYSEMU_SINGLESTEP
#define PTRACE_SYSEMU_SINGLESTEP 0x4400
#endif
#ifndef PTRACE_GET_THREAD_AREA
#define PTRACE_GET_THREAD_AREA 0x4500
#endif
#ifndef PTRACE_SET_THREAD_AREA
#define PTRACE_SET_THREAD_AREA 0x4600
#endif

/* For arm64, these functions don't exist; provide stubs */
struct user_desc;
#ifndef __x86_64__
static inline int ptrace_get_thread_area(struct task_struct *child, int idx,
					 struct user_desc __user *user_desc)
{
	return -38 /* -ENOSYS */;
}
static inline int ptrace_set_thread_area(struct task_struct *child, int idx,
					 struct user_desc __user *user_desc)
{
	return -38 /* -ENOSYS */;
}
#endif

#endif
