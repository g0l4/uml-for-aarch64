/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_VM_FLAGS_ARM64_H
#define __ASM_UM_VM_FLAGS_ARM64_H

/* Arm64 requires PROT_READ for execute-only mappings (BTI/PAC) */
#define VM_DATA_DEFAULT_FLAGS \
	(VM_READ | VM_WRITE | VM_EXEC | \
	 VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

#endif
