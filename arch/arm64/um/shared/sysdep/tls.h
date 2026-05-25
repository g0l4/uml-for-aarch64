/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SYSDEP_TLS_ARM64_H
#define _SYSDEP_TLS_ARM64_H

/* Arm64 uses tpidr_el0 for TLS — no segment-based TLS like x86. */
typedef unsigned long user_desc_t;

extern int os_set_thread_area(user_desc_t *info, int pid);
extern int os_get_thread_area(user_desc_t *info, int pid);

#endif
