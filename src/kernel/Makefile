#
# kernel Makefile
#
# See LICENSE for license details.
# 
# Maintainer : Mathieu Barbe <mathieu.barbe@grenoble-inp.org> - 2018
#
# To view commands use: make V=1
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

DEFAULT_MACHINE = spike
MACHINE ?= $(DEFAULT_MACHINE)

#
# Librairies configuration
#

STDLIB_NAME ?= lib
include ../$(STDLIB_NAME)/Makefile
LIBS = $(LIB_TARGET)

INCLUDE_DIRS = \
	$(LIB_INCLUDES) \
	include \
	asm_include \
	boot/common \

#
# Import specific architecture  parameters :
#

include ../riscv_flags.config

#
# CFlags
#

# Default optim and debug options
CFLAGS ?= -O0 -g

# common architecture option fetched from ../riscv_flags.config
CFLAGS += $(ARCH_CFLAGS)

# Add specific kernel options
CFLAGS += $(addprefix -I, $(INCLUDE_DIRS)) \

#
# LDFLAGS
#

# common architecture option fetched from ../riscv_flags.config
LDFLAGS = $(ARCH_LDFLAGS)

# Add specific kernel LD options
LD_SCRIPT ?= boot/$(MACHINE)/default.lds
LDFLAGS += -T $(LD_SCRIPT)

#
# Kernel sources files
#

KERNEL_SRC_DIRS = \
	. \
	asm_src \

# Fetch BOOT_OBJS
include boot/boot.mk
OBJS = $(BOOT_OBJS) # Fetched from boot/boot.mk

KERNEL_SRC = $(foreach d, $(KERNEL_SRC_DIRS), $(wildcard $(d)/*.c))
KERNEL_ASM = $(foreach d, $(KERNEL_SRC_DIRS), $(wildcard $(d)/*.S))
OBJS += $(sort $(KERNEL_SRC:.c=.o) $(KERNEL_ASM:.S=.o))
OBJS := $(addprefix $(OBJ_DIR)/, $(OBJS))
DEPFILES = $(OBJS:.o=.d)

#
# Targets
#

CONFIG_DEPENDENCIES = Makefile ../riscv_flags.config boot/boot.mk

# Remove every implicite rules
.suffixe:

$(KERNEL_BIN): $(LIBS) $(OBJS) $(LD_SCRIPT)
	$(call cmd, LD $@, $(dir $@), \
	$(LD) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@)

$(OBJ_DIR)/%.o: %.S $(CONFIG_DEPENDENCIES)
	$(call cmd, AS $< -> $@, $(dir $@), \
	$(AS) -c $< -o $@ $(CFLAGS) -MMD -MP)

$(OBJ_DIR)/%.o: %.c $(CONFIG_DEPENDENCIES)
	$(call cmd, CC $< -> $@, $(dir $@), \
	$(CC) -c $< -o $@ $(CFLAGS) -MMD -MP)

# Include object dependencies generated previously by gcc
-include $(DEPFILES)

.PHONY: clean
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

test:
	echo $(LIB_OBJS)
	