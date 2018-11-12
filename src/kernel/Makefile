#
# See LICENSE for license details.
# 
# Maintainer : Mathieu Barbe <mathieu.barbe@grenoble-inp.org>
#

KERNEL_NAME ?= kernel.bin
BUILD_DIR ?= ../build
OBJS_DIR = $(BUILD_DIR)/obj
KERNEL_BIN ?= $(BUILD_DIR)/$(KERNEL_NAME)

all: $(KERNEL_BIN)

#
# Architecture and hardware selection
#

DEFAULT_HOSTARCH = rv64
HOSTARCH ?= $(DEFAULT_HOSTARCH)
DEFAULT_MACHINE = spike
MACHINE ?= $(DEFAULT_MACHINE)

ARCH_DIR = arch/$(HOSTARCH)
MACHINE_DIR = $(ARCH_DIR)/$(MACHINE)

#
# Librairies configuration
#

INCLUDE_DIRS = \
	../libfemto/include \
	$(ARCH_DIR)/common \
	$(MACHINE_DIR)

LIBS = $(BUILD_DIR)/lib/libfemto.a

#
# Import specific architecture  parameters :
# cros-compile tools, CFlags, LDFlags and objects
#

include $(ARCH_DIR)/make.config

#
# CFlags
#

# Default optim and debug options
CFLAGS ?= -O0 -g

# common  arch options
CFLAGS += \
	-mcmodel=medany \
	-ffunction-sections \
	-fdata-sections \
	-nostdinc \

# Add specific arch options (Fetched into arch/xxx/make.config)
CFLAGS += $(KERNEL_ARCH_CFLAGS)

#
# LDFLAGS
#

# common architecture option
LDFLAGS = \
	-nostdlib \
	-nostartfiles \
	-static \
	-lgcc \
	-Wl,--nmagic \
	-Wl,--gc-sections \

# Add specific arch LD options
LDFLAGS += $(KERNEL_ARCH_LDFLAGS)

LD_SCRIPT ?= $(MACHINE_DIR)/default.lds

#
# Kernel sources files
#

# ./kernel/ *.c & *.s
KERNEL_SRC = $(wildcard kernel/*.c)
KERNEL_ASM = $(wildcard kernel/*.s)
KERNEL_OBJS = $(sort $(KERNEL_SRC:.c=.o) $(KERNEL_ASM:.s=.o))

# KERNEL_ARCH_OBJS extrated from./arch/xxx/make.config
OBJS = $(KERNEL_ARCH_OBJS) $(KERNEL_OBJS)

#
# Targets
#

.PHONY: clean
.SUFFIXES: .o .c .s

$(KERNEL_BIN): $(OBJS) $(LD_SCRIPT) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LD_SCRIPT) $(addprefix $(OBJS_DIR)/, $(OBJS)) $(LIBS) -o $@

.c.o:
	mkdir -p $(OBJS_DIR)/$(dir $@)
	$(CC) -c $< -o $(OBJS_DIR)/$@ $(CFLAGS) $(addprefix -I,$(INCLUDE_DIRS))

.s.o:
	mkdir -p $(OBJS_DIR)/$(dir $@)
	$(CC) -c $< -o $(OBJS_DIR)/$@ $(CFLAGS) $(addprefix -I,$(INCLUDE_DIRS))

clean:
	rm -f $(BUILD_DIR)/$(KERNEL_BIN)
	rm -f $(OBJS_DIR)/$(OBJS)

# Todo
#
# To view commands use: make V=1
#

ifdef V
cmd = @mkdir -p $2 ; echo "$3"; $3
else
cmd = @echo "$1"; mkdir -p $2 ; $3
endif

