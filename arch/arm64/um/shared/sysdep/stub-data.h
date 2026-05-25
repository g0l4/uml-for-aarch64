/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ARCH_STUB_DATA_ARM64_H
#define __ARCH_STUB_DATA_ARM64_H

/* Arm64 stub keeps tpidr_el0 (TLS base) across seccomp transitions. */
#define STUB_SYNC_TPIDR	(1 << 0)

struct stub_data_arch {
	int sync;
	unsigned long tpidr_el0;
};

#endif
