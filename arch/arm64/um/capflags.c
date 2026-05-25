// SPDX-License-Identifier: GPL-2.0
#include <linux/types.h>
#include <asm/cpufeatures.h>

/*
 * Minimal capability flag strings for UML-on-arm64.
 * The UML core uses the x86 cpufeature infrastructure to print /proc/cpuinfo
 * flags. Provide empty arrays since arm64 hwcaps are handled separately.
 */
const char * const x86_cap_flags[NCAPINTS * 32] = {
	[0 ... NCAPINTS * 32 - 1] = NULL,
};

const char * const x86_bug_flags[NBUGINTS * 32] = {
	[0 ... NBUGINTS * 32 - 1] = NULL,
};

const char * const x86_power_flags[32] = {
	[0 ... 31] = NULL,
};
