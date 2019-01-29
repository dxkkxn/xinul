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
OUTPUT ?= ../build
OBJ_DIR = $(OUTPUT)/kernel/objs
KERNEL_BIN ?= $(OUTPUT)/$(KERNEL_NAME)

TESTS_OPTIONS ?= WITH_MSG

all: $(KERNEL_BIN)

#
# Architecture and hardware selection
#

DEFAULT_MACHINE = spike
#DEFAULT_MACHINE = qemu-sifive_e
#DEFAULT_MACHINE = qemu-sifive_u
MACHINE ?= $(DEFAULT_MACHINE)

#
# Standard librairies configuration
#

STDLIB_NAME ?= lib
include ../$(STDLIB_NAME)/Makefile
LIBS = $(LIB_TARGET) # Useful to compil stdlib from kernel Makefile


#
# User apps
#

BIN_DIR = $(OUTPUT)/user
include build/apps-integration.mk

#
# Source directories
#

KERNEL_SRC_DIRS = . $(sort $(dir $(wildcard */)))

#
# Include headers directories
#

INCLUDE_DIRS = \
	$(LIB_INCLUDES) \
	$(KERNEL_SRC_DIRS) \
	boot/common \

#
# Kernel sources files
#

# Fetch BOOT_OBJS
include boot/boot.mk
OBJS = $(BOOT_OBJS)

KERNEL_SRC = $(foreach d, $(KERNEL_SRC_DIRS), $(wildcard $(d)/*.c))
KERNEL_ASM = $(foreach d, $(KERNEL_SRC_DIRS), $(wildcard $(d)/*.S))
OBJS += $(sort $(KERNEL_SRC:.c=.o) $(KERNEL_ASM:.S=.o))
OBJS := $(addprefix $(OBJ_DIR)/, $(OBJS))
DEPFILES = $(OBJS:.o=.d)

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

# Add tests options
CFLAGS += $(addprefix -D, $(TESTS_OPTIONS))

#
# LDFLAGS
#

# common architecture option fetched from ../riscv_flags.config
LDFLAGS += $(ARCH_LDFLAGS)

# Add specific kernel LD options
LD_SCRIPTS = boot/$(MACHINE)/default.lds
LDFLAGS += $(addprefix -T, $(LD_SCRIPTS))

#
# Targets
#

CONFIG_DEPENDENCIES = Makefile ../riscv_flags.config boot/boot.mk build/apps-integration.mk

# Remove every implicite rules
.suffixe:

$(KERNEL_BIN): $(LIBS) $(OBJS) $(LD_SCRIPTS) $(APPS_OBJS) $(APPS_OUT)/apps.lds
	$(call cmd, LD $@, $(dir $@), \
	$(LD) $(CFLAGS) $(LDFLAGS) $(OBJS) $(APPS_OBJS) $(LIBS) -o $@)

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



tt:
	echo $(APPS_OBJS)
