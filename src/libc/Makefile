#
# See LICENSE for license details.
# 
# Maintainer : Mathieu Barbe <mathieu.barbe@grenoble-inp.org>
#

LIB_NAME ?= libc
BUILD_DIR ?= ../build/$(LIB_NAME)
OBJS_DIR = $(BUILD_DIR)/obj
LIB ?= $(BUILD_DIR)/$(LIB_NAME).a

all: $(LIB)

#
# Architecture and hardware selection
#

DEFAULT_HOSTARCH = rv64
HOSTARCH ?= $(DEFAULT_HOSTARCH)
ARCH_DIR = arch/$(HOSTARCH)

#
# Import cros-compile tools, CFlags and  LDFlags
#
include ../arch_config/$(HOSTARCH)/make.config

#
# Headers and objects
#

INCLUDE_DIRS = include include/$(ARCH_DIR)

LIB_DIRS = stdin stdlib stdio string drivers $(ARCH_DIR)
LIB_SRC = $(sort $(foreach d, $(LIB_DIRS), $(wildcard $(d)/*.c)))
LIB_ASM = $(sort $(foreach d, $(LIB_DIRS), $(wildcard $(d)/*.s)))
OBJS = $(LIB_SRC:.c=.o) $(LIB_ASM:.s=.o)

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

# Add specific arch C flags (../arch_config/xxx/make.config)
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

#
# Targets
#

.PHONY: clean
.SUFFIXES: .o .c .s

$(LIB): $(OBJS)
	$(AR) cr $@ $(addprefix $(OBJS_DIR)/, $(OBJS))

.c.o:
	mkdir -p $(OBJS_DIR)/$(dir $@)
	$(CC) -c $< -o $(OBJS_DIR)/$@ $(CFLAGS) $(addprefix -I,$(INCLUDE_DIRS))

.s.o:
	mkdir -p $(OBJS_DIR)/$(dir $@)
	$(CC) -c $< -o $(OBJS_DIR)/$@ $(CFLAGS) $(addprefix -I,$(INCLUDE_DIRS))

clean:
	rm -f $(LIB)
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

