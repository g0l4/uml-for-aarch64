// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Ant Group
 * Author: Tiwei Bie <tiwei.btw@antgroup.com>
 */

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <kern_util.h>
#include <um_malloc.h>
#include <init.h>
#include <os.h>
#include <smp.h>
#include "internal.h"

struct cpu_thread_data {
	int cpu;
	sigset_t sigset;
};

static __thread int __curr_cpu;

int uml_curr_cpu(void)
{
	return __curr_cpu;
}

static pthread_t cpu_threads[CONFIG_NR_CPUS];
static int cpu_tids[CONFIG_NR_CPUS];
static int tgid;

static void *cpu_thread(void *arg)
{
	struct cpu_thread_data *data = arg;

	__curr_cpu = data->cpu;
	cpu_tids[data->cpu] = syscall(SYS_gettid);

	uml_start_secondary(data);

	return NULL;
}

int os_start_cpu_thread(int cpu)
{
	struct cpu_thread_data *data;
	sigset_t sigset, oset;
	int err;

	data = uml_kmalloc(sizeof(*data), UM_GFP_ATOMIC);
	if (!data)
		return -ENOMEM;

	sigfillset(&sigset);
	if (sigprocmask(SIG_SETMASK, &sigset, &oset) < 0) {
		err = errno;
		goto err;
	}

	data->cpu = cpu;
	data->sigset = oset;

	err = pthread_create(&cpu_threads[cpu], NULL, cpu_thread, data);
	if (sigprocmask(SIG_SETMASK, &oset, NULL) < 0)
		panic("Failed to restore the signal mask, errno = %d", errno);
	if (err != 0)
		goto err;

	return 0;

err:
	kfree(data);
	return -err;
}

void os_start_secondary(void *arg, jmp_buf *switch_buf)
{
	struct cpu_thread_data *data = arg;

	sigaddset(&data->sigset, IPI_SIGNAL);
	sigaddset(&data->sigset, SIGIO);

	if (sigprocmask(SIG_SETMASK, &data->sigset, NULL) < 0)
		panic("Failed to restore the signal mask, errno = %d", errno);

	kfree(data);
	longjmp(*switch_buf, 1);

	/* unreachable */
	printk(UM_KERN_ERR "impossible long jump!");
	fatal_sigsegv();
}

int os_send_ipi(int cpu, int vector)
{
	siginfo_t info;

	memset(&info, 0, sizeof(info));
	info.si_signo = IPI_SIGNAL;
	info.si_code = SI_QUEUE;
	info.si_pid = tgid;
	info.si_uid = getuid();
	info.si_value.sival_int = vector;

	return syscall(SYS_rt_tgsigqueueinfo, tgid, cpu_tids[cpu],
		       IPI_SIGNAL, &info);
}

static void __local_ipi_set(int enable)
{
	sigset_t sigset;

	sigemptyset(&sigset);
	sigaddset(&sigset, IPI_SIGNAL);

	if (sigprocmask(enable ? SIG_UNBLOCK : SIG_BLOCK, &sigset, NULL) < 0)
		panic("%s: sigprocmask failed, errno = %d", __func__, errno);
}

void os_local_ipi_enable(void)
{
	__local_ipi_set(1);
}

void os_local_ipi_disable(void)
{
	__local_ipi_set(0);
}

static void ipi_sig_handler(int sig, siginfo_t *si, void *uc)
{
	int save_errno = errno;

	signals_enabled = 0;
	um_trace_signals_off();

	uml_ipi_handler(si->si_value.sival_int);

	um_trace_signals_on();
	signals_enabled = 1;

	errno = save_errno;
}

void __init os_init_smp(void)
{
	struct sigaction action = {
		.sa_sigaction = ipi_sig_handler,
		.sa_flags = SA_SIGINFO | SA_ONSTACK | SA_RESTART,
	};

	sigfillset(&action.sa_mask);

	if (sigaction(IPI_SIGNAL, &action, NULL) < 0)
		panic("%s: sigaction failed, errno = %d", __func__, errno);

	tgid = os_getpid();
	cpu_threads[0] = pthread_self();
	cpu_tids[0] = syscall(SYS_gettid);
}
