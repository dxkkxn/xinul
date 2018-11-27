#
# See LICENSE for license details.
# 
# Maintainer : Mathieu Barbe <mathieu.barbe@grenoble-inp.org>
#

SHELL = /bin/sh
KERNEL_NAME ?= kernel.bin
BUILD_DIR ?= ../build
OBJ_DIR = $(BUILD_DIR)/kernel_objs
KERNEL_BIN ?= $(BUILD_DIR)/$(KERNEL_NAME)

all: $(KERNEL_BIN)

#
# Architecture and hardware selection
#

DEFAULT_HOSTARCH = rv64
HOSTARCH ?= $(DEFAULT_HOSTARCH)
DEFAULT_MACHINE = spike
MACHINE ?= $(DEFAULT_MACHINE)

#
# Librairies configuration
#

LIBC_NAME ?= libc
LIBC_INCLUDE ?= ../$(LIBC_NAME)/include
LIBC ?= $(BUILD_DIR)/$(LIBC_NAME).a

LIBS = $(LIBC)

INCLUDE_DIRS = \
	. \
	$(LIBC_INCLUDE) \
	boot/common \
	boot/$(MACHINE)


#
# Import specific architecture  parameters :
#

# Import cros-compile tools, CFlags and  LDFlags
include ../arch_config/$(HOSTARCH)/make.config

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
CFLAGS += $(ARCH_CFLAGS)

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
LDFLAGS += $(ARCH_LDFLAGS)

LD_SCRIPT ?= boot/$(MACHINE)/default.lds

#
# Kernel sources files
#

# Fetch boot objects in BOOT_OBJS
include boot/make.config

KERNEL_SRC = $(wildcard *.c)
KERNEL_ASM = $(wildcard *.s)
OBJS = $(sort $(BOOT_OBJS) $(KERNEL_SRC:.c=.o) $(KERNEL_ASM:.s=.o))
OBJS := $(addprefix $(OBJ_DIR)/, $(OBJS))

#
# Targets
#

# Remove every implicite rules
.suffixe:

.PHONY: clean

test:
	echo $(OBJS)

$(KERNEL_BIN): $(LIBS) $(OBJS) $(LD_SCRIPT)
	$(call cmd, LD $@, $(dir $@), \
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LD_SCRIPT) $(OBJS) $(LIBS) -o $@)

$(OBJ_DIR)/%.o: %.s Makefile
	$(call cmd, CC $< -> $@, $(dir $@), \
	$(CC) -c $< -o $@ $(CFLAGS) $(addprefix -I, $(INCLUDE_DIRS)))

$(OBJ_DIR)/%.o: %.c Makefile
	$(call cmd, CC $< -> $@, $(dir $@), \
	$(CC) -c $< -o $@ $(CFLAGS) $(addprefix -I, $(INCLUDE_DIRS)))

clean:
	rm -f $(KERNEL_BIN)
	rm -rf $(OBJ_DIR)

#
# To view commands use: make V=1
#

ifdef V
cmd = @mkdir -p $2 ; echo "$3"; $3
else
cmd = @echo "$1"; mkdir -p $2 ; $3
endif
