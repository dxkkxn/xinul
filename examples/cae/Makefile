# a minimal example to get RISC-V bare metal assembler running on spike

DK_IMAGE_NAME=riscv
DK_DOCKERFILE_DIR=../../docker/riscv-env
DK_MNT_DIR=/mnt/host
DK_CMD=/bin/bash

dk-build:
	docker image build -t ${DK_IMAGE_NAME} ${DK_DOCKERFILE_DIR}

dk-run:
	docker run -ti --rm -v ${PWD}:${DK_MNT_DIR} -w ${DK_MNT_DIR} ${DK_IMAGE_NAME} ${DK_CMD}
doit:
	riscv64-unknown-elf-gcc -march=rv64g -mabi=lp64 -static -mcmodel=medany -fvisibility=hidden -nostdlib -nostartfiles -Tlinker.ld test.s -o test
	spike --isa=rv64gc -d test
