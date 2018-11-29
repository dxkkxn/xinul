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
	$(LIBC_INCLUDE) \
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
LDFLAGS += 

LD_SCRIPT ?= boot/$(MACHINE)/default.lds

#
# Kernel sources files
#

SRC_DIRS = \
. \
boot/common \
boot/$(MACHINE) \

KERNEL_SRC = $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c))
KERNEL_ASM = $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.s))
OBJS = $(sort $(KERNEL_SRC:.c=.o) $(KERNEL_ASM:.s=.o))
OBJS := $(addprefix $(OBJ_DIR)/, $(OBJS))
DEPFILES = $(OBJS:.o=.d)

# Import .s dependencies (-mdp does not work with .s files)
include boot/make.config

#
# Targets
#

# Remove every implicite rules
.suffixe:

.PHONY: clean

# Include object dependencies generated previously by gcc
-include $(DEPFILES)


$(KERNEL_BIN): $(LIBS) $(OBJS) $(LD_SCRIPT)
	$(call cmd, LD $@, $(dir $@), \
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LD_SCRIPT) $(OBJS) $(LIBS) -o $@)

$(OBJ_DIR)/%.o: %.s Makefile
	$(call cmd, CC $< -> $@, $(dir $@), \
	$(CC) -c $< -o $@ $(CFLAGS) -MMD -MP)


$(OBJ_DIR)/%.o: %.c Makefile
	$(call cmd, CC $< -> $@, $(dir $@), \
	$(CC) -c $< -o $@ $(CFLAGS) -MMD -MP)

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
