DEFAULT_HOSTARCH = rv64
HOSTARCH ?= $(DEFAULT_HOSTARCH)
DEFAULT_MACHINE = spike
MACHINE ?= $(DEFAULT_MACHINE)

#
# Import specific arch parameters : 
# - cros-compile tools, arch cflags and ldflags, objs
#
ARCHDIR = arch/$(HOSTARCH)
MACHINE_DIR = $(ARCHDIR)/$(MACHINE)
include $(ARCHDIR)/make.config

CC = $(CROSS_COMPILE)gcc



#
# Flags
#

CFLAGS ?= -O0 -g
CFLAGS += \
-mcmodel=medany \
-ffunction-sections \
-fdata-sections \

LDFLAGS = \
-nostdlib \
-nostartfiles \
-nostdinc \
-static \
-Wl,--nmagic \
-Wl,--gc-sections \

LIBS := $(LIBS) \
-lgcc
# -lk
#
# Outpout directory
#
DESTDIR ?= sysroot
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR? = $(EXEC_PREFIX)/boot

INCLUDEDIR ?= $(PREFIX)/include



CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS := $(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/start.o \

OBJS = \
$(CRT_OBJS) \
$(KERNEL_OBJS) \

LINK_LIST = \
$(LDFLAGS) \
$(CRT_OBJS) \
$(KERNEL_OBJS) \
$(LIBS) \

LD_SCRIPT ?= $(MACHINE_DIR)/default.lds

#
# Targets
#

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .s

all: kernel.bin

kernel.bin: $(OBJS) $(LD_SCRIPT)
	$(CC) -T $(LD_SCRIPT) $(LINK_LIST) $(CFLAGS) -o $@

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	$(CC) -c $< -o $@ $(CFLAGS)

.s.o:
	$(CC) -c $< -o $@ $(CFLAGS) -Iarch/rv64/common

clean:
	rm -f myos.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
