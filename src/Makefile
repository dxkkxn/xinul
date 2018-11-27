
KERNEL_NAME = kernel.bin
LIBC_NAME = libc
BUILD_DIR = build
KERNEL_BIN = $(BUILD_DIR)/$(KERNEL_NAME)
LIBC = $(BUILD_DIR)/$(LIBC_NAME)/$(LIBC_NAME).a

# Qemu
# Qemu environement machine : spike_v1.10
QEMU_MACHINE = spike_v1.10

QEMU = qemu-system-riscv64
QEMU_OPTS = -machine $(QEMU_MACHINE) -kernel $(KERNEL_BIN) -rtc base=localtime -k fr -m 256M
QEMU_GDB_OPTS = -s -S
# Pour afficher l'affichage VGA en console, marche à travers ssh
QEMU_NO_GRAPH_OPTS = -nographic

# X-compile
CC=riscv64-unknown-elf-gcc
GDB=riscv64-unknown-elf-gdb

# Docker
DK_MAKEFILE_DIR = ../docker/riscv/
DK_SHARE_DIR = $(PWD)

#
# Targets
#

.PHONY: clean $(KERNEL_BIN) $(LIBC)

all:$(KERNEL_BIN)

$(KERNEL_BIN): $(LIBC)
	make -C kernel/ all	

$(LIBC):
	make -C $(LIBC_NAME)

clean:
	rm -rf $(BUILD_DIR)

# Lance l'OS dans un terminal et attend gdb.
run: all
	$(QEMU) $(QEMU_OPTS) $(QEMU_GDB_OPTS) $(QEMU_NO_GRAPH_OPTS)

# Lance l'OS sans gdb en mode console.
go: all
	$(QEMU) $(QEMU_OPTS) $(QEMU_NO_GRAPH_OPTS)


# Lance l'OS en mode graphique
go-graphic: all
	$(QEMU) $(QEMUOPTS)

# Ferme qemu sur l'ordinateur.
kill:
	killall $(QEMU) 

kill9:
	killall -9 $(QEMU) 

# Lancement de GDB
gdb:
	$(GDB) $(KERNEL_BIN)

# make accessible
make aall:
	(make 2> out && echo "Built success" ) || nano out

# Target Docker
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


