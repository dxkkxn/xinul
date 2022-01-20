# Benoît Wallon <benoit.wallon@grenoble-inp.org> - 2019
# Mathieu Barbe <mathieu@kolabnow.com> - 2019
# Frédéric Pétrot <frederic.petrot@univ-grenoble-alpes.fr> - 2022
# See license for license details.
# a minimal example to test X-compile in host or in Docker container

DK_IMAGE_NAME = barbem/risc-v_cep
DK_DOCKERFILE_DIR=../../docker
DK_MNT_DIR=/mnt/host
DK_RUN_CMD=docker run --rm -v ${PWD}:${DK_MNT_DIR} -w ${DK_MNT_DIR} ${DK_IMAGE_NAME}

include ../../src/riscv_flags.config

CC=riscv64-unknown-elf-gcc
# Do not use -O0 as it uses s0 although I try to prevent it
CFLAGS=-gdwarf -O2 ${ARCH_CFLAGS} -I../../src/lib/include -ffreestanding -ffixed-s0
LDSCRIPT=default.ld
LDFLAGS=${ARCH_LDFLAGS} -ffreestanding

all:main
	file main

dk-pull:${DK_DOCKERFILE_DIR}/Dockerfile
	docker pull ${DK_IMAGE_NAME}

dk-all:
	${DK_RUN_CMD} make all

crtm.o:crtm.S
	${CC} ${CFLAGS} -c $<
context_switch.o:context_switch.S
	${CC} ${CFLAGS} -c $<
main.o:main.c
	${CC} ${CFLAGS} -c $<
main:main.o context_switch.o crtm.o
	${CC} ${LDFLAGS} -T ${LDSCRIPT} $^ -o $@

clean:
	${RM} main main.o context_switch.o crtm.o
