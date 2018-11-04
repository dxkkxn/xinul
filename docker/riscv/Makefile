
DK_IMAGE_NAME=riscv
DK_DOCKERFILE_DIR=.
DK_MNT_DIR=/mnt/host
DK_RUN_CMD=docker run -ti --rm -v ${PWD}:${DK_MNT_DIR} -w ${DK_MNT_DIR} ${DK_IMAGE_NAME}

built:.image-built 

.image-built:${DK_DOCKERFILE_DIR}/Dockerfile
	docker image build -t ${DK_IMAGE_NAME} ${DK_DOCKERFILE_DIR}
	touch .image-built

bash:.image-built
	${DK_RUN_CMD} /bin/bash

clean:
	docker image rm ${DK_IMAGE_NAME}; \
	rm .image-built
