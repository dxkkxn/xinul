#
# # User space compile tool chain.
#
# See LICENSE for license details.
# 
# Copyright (C) 
# 2012-2014 - Damien Dejean <dam.dejean@gmail.com>
# 2018 - Mathieu Barbe <mathieu.barbe@grenoble-inp.org> - 2018
#
# This Makefile use sub makefiles in build/ to build and link each app and
# the standard library for our custom kernel/userspace environment.
#
# To view commands use: make V=1
#

SHELL = /bin/sh
BUILD_DIR ?= ../build
OUTPUT ?= $(BUILD_DIR)/user

user_space: apps

### Platform tools ###
#$(PLATFORM_TOOLS): | $(OUTPUT)
#	build/generate-platform-tools.sh > $@

#
# Common cflags and ldflags
#

# Default optim and debug options
CFLAGS ?= -O0 -g

# Import specific architecture  parameters :
include ../riscv_flags.config
CFLAGS += $(ARCH_CFLAGS)
LDFLAGS = $(ARCH_LDFLAGS)
LDFLAGS += $(BUILD_DIR)/ulib.a

# todo a regarder si utile
# Include makefile utilities
include build/utils.mk

#
### Import standard library build toolchain ###
#

STDLIB_NAME ?= lib
include ../$(STDLIB_NAME)/Makefile

#
# Import userspace program build toolchain
#

APPS_NAMES      = hello
APPS_STD_INC    = $(LIB_INCLUDES) # Fetched from ../lib/Makefile
APPS_STD_LIB = $(BUILD_DIR)/$(STDLIB_NAME).a
APPS_STD_TARGET := $(LIB_TARGET)
include build/apps.mk

#
# The all target is virtual, it is just a way to call all
#

apps: $(APPS_TARGETS) ulib

ulib:
	make -C ulib/Makefile

### Standard output directory ###
$(OUTPUT):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(OUTPUT)

testas:
	echo $(AS)