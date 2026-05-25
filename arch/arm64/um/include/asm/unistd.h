/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_UNISTD_ARM64_H
#define __ASM_UM_UNISTD_ARM64_H

/*
 * UML-on-arm64 unistd.h.
 * Provides __ARCH_WANT_* macros needed by the generic syscall implementations
 * and includes the generated syscall number definitions.
 */

#define __ARCH_WANT_SYS_CLONE
#define __ARCH_WANT_NEW_STAT

#include <asm/unistd_64.h>

#define NR_syscalls (__NR_syscalls)

#endif
