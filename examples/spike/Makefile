# a minimal example to compile C program and run it in Spike

DK_IMAGE_NAME=riscv-src
DK_DOCKERFILE_DIR=../../docker/riscv-env-src
DK_MNT_DIR=/mnt/host
DK_RUN_CMD=docker run -ti --rm -v ${PWD}:${DK_MNT_DIR} -w ${DK_MNT_DIR} ${DK_IMAGE_NAME}

CC=riscv64-unknown-elf-gcc

all:spike

dk-build:${DK_DOCKERFILE_DIR}/Dockerfile
	docker image build -t ${DK_IMAGE_NAME} ${DK_DOCKERFILE_DIR}

dk-bash:.
	${DK_RUN_CMD} /bin/bash

dk-all:hello.c
	${DK_RUN_CMD} make spike

build:hello
hello:hello.c
	${CC} hello.c -o hello 

spike:hello
	spike pk hello

clean:
	rm -f hello
