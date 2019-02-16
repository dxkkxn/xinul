#
# Global Makefile
#
# See LICENSE for license details.
# 
# Mathieu Barbe <mathieu.barbe@grenoble-inp.org> - 2018 - 2019
#
# To view commands use: make V=1
#

KERNEL_NAME = kernel.bin
BUILD_DIR = build
KERNEL_BIN = $(BUILD_DIR)/$(KERNEL_NAME)

#
# Machine
#

MACHINE ?= sifive_u
#MACHINE ?= spike
#MACHINE ?= zybo

#
# Hardware plateforme
#

ZYBO_IP = 192.168.1.5
ZYBO_USER = root
ZYBO_MDP = root

#
# GDB
#

GDB=riscv64-unknown-elf-gdb
GDBINIT_PATH = ./gdbinit
GDB_OPTS = -q -x $(GDBINIT_PATH)

#
# Qemu
#

QEMU = qemu-system-riscv64
ifeq ("$(MACHINE)", "spike")
	QEMU_MACHINE=spike_v1.10
else
	ifeq ("$(MACHINE)", "sifive_u")
		QEMU_MACHINE=sifive_u
	else
		$(error Machine unknown)
	endif
endif

QEMU_OPTS = -machine $(QEMU_MACHINE) -kernel $(KERNEL_BIN) -rtc base=localtime -k fr -m 256M
QEMU_GDB_OPTS = -s -S
# Pour afficher l'affichage VGA en console, marche à travers ssh
QEMU_NO_GRAPH_OPTS = -nographic

#
# Docker
#

DK_MAKEFILE_DIR = ../docker/
DK_SHARE_DIR = $(PWD)

#
# Targets
#

.PHONY: clean $(KERNEL_BIN) user

all: kernel

kernel: user
	make MACHINE=$(MACHINE) -C kernel/ all

user:
	make -C user

stdlib:
	make -C $(STDLIB_NAME)

clean:
	rm -rf $(BUILD_DIR)

#
# Simulation
#

# Go :Lance l'OS avec QEmu dans le terminal
go: all
	$(QEMU) $(QEMU_OPTS) $(QEMU_NO_GRAPH_OPTS)

# Run : Lance l'OS dans un terminal avec QEmu et attend gdb.
run: all
	$(QEMU) $(QEMU_OPTS) $(QEMU_GDB_OPTS) $(QEMU_NO_GRAPH_OPTS)

# Go-graphic : Lance l'OS avec QEmu en mode graphique
go-graphic: all
	$(QEMU) $(QEMUOPTS)

# kill QEmu si CTRL-A, X ne fonctionne pas.
kill_qemu:
	killall -9 $(QEMU) 

# GDB : Lancement de GDB
gdb:
	$(GDB) $(GDB_OPTS) $(KERNEL_BIN)

#
# Target Docker
#

dk-image-dl: $(DK_MAKEFILE_DIR)/Makefile
	make -C $(DK_MAKEFILE_DIR) pull

dk-image-build: $(DK_MAKEFILE_DIR)/Makefile
	make -C $(DK_MAKEFILE_DIR) build

dk-bash: $(DK_MAKEFILE_DIR)/Makefile
	make -C $(DK_MAKEFILE_DIR) bash

dk-all: $(DK_MAKEFILE_DIR)/Makefile
	make -C $(DK_MAKEFILE_DIR) CMD="make all" exec

dk-go: $(DK_MAKEFILE_DIR)/Makefile
	make -C $(DK_MAKEFILE_DIR) CMD="make go" exec

dk-rm-image: $(DK_MAKEFILE_DIR)/Makefile
	make -C $(DK_MAKEFILE_DIR) CMD="make clean" exec

#
# Targets pour plateforme Zybo
#

zybo: user
	make MACHINE=zybo -C kernel/ all

zybo_ssh:
	ssh $(ZYBO_USER)@$(ZYBO_IP)

zybo_copy: zybo
	scp $(KERNEL_BIN) $(ZYBO_USER)@$(ZYBO_IP):

zybo_go:zybo_copy
	ssh $(ZYBO_USER)@$(ZYBO_IP) ./fesvr-zynq $(KERNEL_NAME)

#
# Machine Spike (Simulateur fonctionel)
#

spike: user
	make MACHINE=spike -C kernel/ all

spike_go: spike
	spike $(KERNEL_BIN)

spike_qemu:
	make MACHINE=spike -C . go
