// SPDX-License-Identifier: GPL-2.0
#include <stddef.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <linux/elf.h>
#include <sysdep/tls.h>
#include <registers.h>

int os_set_thread_area(user_desc_t *info, int pid)
{
	struct iovec iov = {
		.iov_base = info,
		.iov_len = sizeof(*info),
	};

	if (ptrace(PTRACE_SETREGSET, pid, NT_ARM_TLS, &iov) < 0)
		return -errno;

	return 0;
}

int os_get_thread_area(user_desc_t *info, int pid)
{
	struct iovec iov = {
		.iov_base = info,
		.iov_len = sizeof(*info),
	};

	if (ptrace(PTRACE_GETREGSET, pid, NT_ARM_TLS, &iov) < 0)
		return -errno;

	return 0;
}
