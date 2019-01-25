#
# Ensimag - Projet système
# Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
#

#
# Generates object files from userspace binaries and integrates them in the
# kernel binary. Generates a table to be able to find theses programs in
# the binary when the kernel is loaded.
#
# Dependencies:
#  OUTPUT
# BIN_DIR : Directory when user binaries is generated.
# Provides:
#  APPS_OBJS: The application object files

# Defines build environment
GEN_SECTIONS := build/generate-link-sections.sh
GEN_TABLE    := build/generate-symbols-table.sh


BIN_DIRS := $(BIN_DIR) $(BIN_DIR)/tests
APPS_BIN := $(foreach d, $(BIN_DIRS), $(wildcard $(d)/*.bin))
APPS_OUT  := $(OUTPUT)/kernel
APPS_OBJS := $(APPS_BIN:$(BIN_DIR)/%=$(APPS_OUT)/%.o)

APPS_OBJS  += $(APPS_OUT)/symbols-table.o
vpath %.bin $(dir $(APPS_BIN))

# Transform binary files in linkable files
$(APPS_OUT)/%.bin.o: $(OBJ_DIR)/empty.o $(OUTPUT)/user/%.bin | $(APPS_OUT) $(APPS_OUT)/tests
	$(OBJCOPY) $< \
		--add-section=.$(notdir $*).bin=$(filter-out $<, $^) \
		--set-section-flags=.$*.bin=contents,alloc,load,data $@

$(APPS_OUT) $(APPS_OUT)/tests:
	mkdir -p $@/tests

# Generate a linker file that:
# - creates a table with an entry (address, size) for each application
# - include each binary in its own section
# This is a intermediate target that will be automagically deleted by make
# at the end of the build. It ensures that the script is entirely
# re-generated at each build.

$(APPS_OUT)/symbols-table.o: $(APPS_OUT)/symbols-table.c | $(APPS_OUT)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: $(APPS_OUT)/symbols-table.c
#.INTERMEDIATE: $(APPS_OUT)/symbols-table.c
$(APPS_OUT)/symbols-table.c: | $(APPS_OUT)
	$(GEN_TABLE) "$@" $(APPS_BIN)

.PHONY: $(APPS_OUT)/apps.lds
$(APPS_OUT)/apps.lds: | $(APPS_OUT)
	$(GEN_SECTIONS) "$@" $(APPS_BIN)

#.PHONY: build/kernel.lds
#build/kernel.lds: $(APPS_OUT)/apps.lds

# Add the apps dir to LDFLAGS to ensure the linker will find our script
LDFLAGS += -L$(APPS_OUT)/

