/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_SEGMENT_ARM64_H
#define __ASM_UM_SEGMENT_ARM64_H

/*
 * Arm64 has no segmentation. Provide minimal stubs for the UML
 * generic code which expects these definitions.
 */
#define GDT_ENTRY_TLS_ENTRIES 1

#endif
