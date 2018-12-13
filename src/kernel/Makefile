# # See LICENSE for license details.  # # Maintainer : Mathieu Barbe <mathieu.barbe@grenoble-inp.org>
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
STDLIB_INCLUDE ?= \
	../$(STDLIB_NAME)/include \
	../$(STDLIB_NAME)/include/arch/riscv

STDLIB ?= $(BUILD_DIR)/$(STDLIB_NAME).a

LIBS = $(STDLIB)

INCLUDE_DIRS = \
	$(STDLIB_INCLUDE) \
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

SRC_DIRS = \
	. \
	asm_src \

KERNEL_SRC = $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c))
KERNEL_ASM = $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.S))
OBJS = $(sort $(KERNEL_SRC:.c=.o) $(KERNEL_ASM:.S=.o))
include boot/make.config # Fetch BOOT_OBJS and them dependencies.
OBJS += $(BOOT_OBJS)
OBJS := $(addprefix $(OBJ_DIR)/, $(OBJS))
DEPFILES = $(OBJS:.o=.d)

#
# Targets
#

# Remove every implicite rules
.suffixe:

.PHONY: clean $(LIBS)

$(KERNEL_BIN): $(LIBS) $(OBJS) $(LD_SCRIPT)
	$(call cmd, LD $@, $(dir $@), \
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@)

$(OBJ_DIR)/%.o: %.S Makefile ../riscv_flags.config
	$(call cmd, CC $< -> $@, $(dir $@), \
	$(CC) -c $< -o $@ $(CFLAGS) -MMD -MP)

$(OBJ_DIR)/%.o: %.c Makefile ../riscv_flags.config
	$(call cmd, CC $< -> $@, $(dir $@), \
	$(CC) -c $< -o $@ $(CFLAGS) -MMD -MP)

# Include object dependencies generated previously by gcc
-include $(DEPFILES)
 
$(STDLIB):
	make -C ../$(STDLIB_NAME)

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
