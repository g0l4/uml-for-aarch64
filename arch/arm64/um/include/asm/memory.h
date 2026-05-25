/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_MEMORY_ARM64_H
#define __ASM_UM_MEMORY_ARM64_H

/*
 * UML-on-arm64 memory layout stubs.
 * UML manages its own virtual memory — these are provided to satisfy
 * includes that pull in the native arm64 memory.h.
 */

#define VA_BITS		48
#define PAGE_OFFSET	((unsigned long)0xffff000000000000UL)
#define PAGE_END	((unsigned long)0xffffffffffffffffUL)

#endif
