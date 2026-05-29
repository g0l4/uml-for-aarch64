// SPDX-License-Identifier: GPL-2.0
#include <signal.h>
#include <sysdep/stub.h>
#include <sysdep/faultinfo.h>
#include <sysdep/mcontext.h>
#include <sys/ucontext.h>

/*
 * The SEGV handler installed in the stub child process.
 * This runs on the stub's signal stack and communicates the
 * fault back to the UML kernel via the shared stub_data page.
 */
void __attribute__((__section__(".syscall_stub")))
stub_segv_handler(int sig, siginfo_t *info, void *p)
{
	struct faultinfo *f = get_stub_data();
	ucontext_t *uc = p;

	GET_FAULTINFO_FROM_MC(*f, &uc->uc_mcontext);
	trap_myself();
}
