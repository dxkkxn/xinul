# a minimal example to test X-compile in host or in Docker container

DK_IMAGE_NAME=risc-v
DK_DOCKERFILE_DIR=../../docker/riscv
DK_MNT_DIR=/mnt/host
DK_RUN_CMD=docker run --rm -v ${PWD}:${DK_MNT_DIR} -w ${DK_MNT_DIR} ${DK_IMAGE_NAME}

CC=riscv64-unknown-elf-gcc

all:main
	file main

dk-build:${DK_DOCKERFILE_DIR}/Dockerfile
	docker image build -t ${DK_IMAGE_NAME} ${DK_DOCKERFILE_DIR}

dk-all: dk-build
	${DK_RUN_CMD} make all

main:main.c
	${CC} main.c -o main

clean:
	rm -f main
