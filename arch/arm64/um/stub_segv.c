// SPDX-License-Identifier: GPL-2.0
#include <signal.h>
#include <sysdep/stub.h>

/*
 * The SEGV handler installed in the stub child process.
 * This runs on the stub's signal stack and communicates the
 * fault back to the UML kernel via the shared stub_data page.
 */
void __attribute__((__naked__)) stub_segv_handler(int sig, siginfo_t *info, void *p)
{
	/*
	 * The stub SEGV handler is always set up by the UML kernel
	 * through the generic SKAS stub infrastructure.
	 * The actual trampoline is architecture-generic.
	 */
	__asm__("b stub_signal_interrupt");
}
