// SPDX-License-Identifier: GPL-2.0
#include <errno.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <elf.h>
#ifndef NT_ARM_FPSIMD
#define NT_ARM_FPSIMD 0x402
#endif
#include <longjmp.h>
#include <sysdep/ptrace_user.h>
#include <registers.h>
#include <sys/mman.h>
#include <asm/ptrace.h>

static unsigned long ptrace_regset;
unsigned long host_fp_size;

int get_fp_registers(int pid, unsigned long *regs)
{
	struct iovec iov = {
		.iov_base = regs,
		.iov_len = host_fp_size,
	};

	if (ptrace(PTRACE_GETREGSET, pid, ptrace_regset, &iov) < 0)
		return -errno;
	return 0;
}

int put_fp_registers(int pid, unsigned long *regs)
{
	struct iovec iov = {
		.iov_base = regs,
		.iov_len = host_fp_size,
	};

	if (ptrace(PTRACE_SETREGSET, pid, ptrace_regset, &iov) < 0)
		return -errno;
	return 0;
}

int arch_init_registers(int pid)
{
	struct iovec iov = {
		.iov_len = 2 * 1024 * 1024,
	};
	int ret;

	iov.iov_base = mmap(NULL, iov.iov_len, PROT_WRITE | PROT_READ,
			    MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (iov.iov_base == MAP_FAILED)
		return -ENOMEM;

	/* Try SVE first (NT_ARM_SVE), fall back to FPSIMD (NT_ARM_FPSIMD) */
	ptrace_regset = NT_ARM_SVE;
	ret = ptrace(PTRACE_GETREGSET, pid, ptrace_regset, &iov);
	if (ret) {
		ptrace_regset = NT_ARM_FPSIMD;
		iov.iov_len = 2 * 1024 * 1024;
		ret = ptrace(PTRACE_GETREGSET, pid, ptrace_regset, &iov);
		if (ret)
			ret = -errno;
	}

	munmap(iov.iov_base, 2 * 1024 * 1024);

	host_fp_size = iov.iov_len;

	return ret;
}

unsigned long get_thread_reg(int reg, jmp_buf *buf)
{
	switch (reg) {
	case HOST_PC:
		return buf[0]->__lr;
	case HOST_SP:
		return buf[0]->__sp;
	case HOST_X29:
		return buf[0]->__fp;
	default:
		printk(UM_KERN_ERR "get_thread_regs - unknown register %d\n", reg);
		return 0;
	}
}
