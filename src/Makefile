
KERNEL_BIN=kernel.bin
# Machine : spike_v1.10
QEMU_MACHINE = spike_v1.10

QEMU = qemu-system-riscv64
QEMU_OPTS = -machine $(QEMU_MACHINE) -kernel $(KERNEL_BIN) -rtc base=localtime -k fr 
QEMU_GDB_OPTS = -s -S
# Pour afficher l'affichage VGA en console, marche à travers ssh
QEMU_NO_GRAPH_OPTS = -nographic

CC=riscv64-unknown-elf-gcc
GDB=riscv64-unknown-elf-gdb

all:$(KERNEL_BIN)

clean:
	rm -rf $(KERNEL_BIN) .image-built

$(KERNEL_BIN):
	touch kernel.bin

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
	$(GDB) $(KERNEL_bin)

# make accessible
make aall:
	(make 2> out && echo "Built success" ) || nano out

# Docker
DK_IMAGE_NAME=riscv-src
DK_DOCKERFILE_DIR=../docker/riscv-env-src/
DK_MNT_DIR=/mnt/host
DK_RUN_CMD=docker run -ti --rm -v ${PWD}:${DK_MNT_DIR} -w ${DK_MNT_DIR} ${DK_IMAGE_NAME}
DK_CMD=

dk-build:.image-built

.image-built:$(DK_DOCKERFILE_DIR)/Dockerfile
	docker image build -t ${DK_IMAGE_NAME} ${DK_DOCKERFILE_DIR}
	touch .image-built

dk-bash:.image-built
	${DK_RUN_CMD} /bin/bash

dk-make::.image-built
	${DK_RUN_CMD} make $(TARGET)

dk-clean:
	rm -f .image-built

dk-rm-image:dk-clean
	docker image rm ${DK_IMAGE_NAME}; \

