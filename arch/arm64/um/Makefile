# SPDX-License-Identifier: GPL-2.0
#
# UML-on-arm64 subarchitecture Makefile
#

obj-y = bugs_64.o capflags.o delay.o fault.o ptrace.o ptrace_user.o \
	setjmp_64.o signal.o stub_segv.o sys_call_table_64.o sysrq_64.o \
	os-Linux/

obj-y += syscalls_64.o mem_64.o

subarch-y =

subarch-$(CONFIG_MODULES) += ../kernel/module.o

USER_OBJS := bugs_64.o ptrace_user.o fault.o

$(obj)/user-offsets.s: c_flags = -Wp,-MD,$(depfile) $(USER_CFLAGS) \
	-Iarch/arm64/include/generated
targets += user-offsets.s

include/generated/user_constants.h: $(obj)/user-offsets.s FORCE
	$(call filechk,offsets,__USER_CONSTANT_H__)

UNPROFILE_OBJS := stub_segv.o
CFLAGS_stub_segv.o := $(CFLAGS_NO_HARDENING)

include $(srctree)/arch/um/scripts/Makefile.rules
