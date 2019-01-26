
BOOT_OBJS = \
	boot/common/supervisor.o \
	boot/common/boot.o \
	boot/$(MACHINE)/crt.o \
	boot/$(MACHINE)/setup.o \
