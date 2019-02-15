
DK_IMAGE_NAME=raifer/risc-v
DK_DOCKERFILE_DIR=.
DK_MNT_DIR=/mnt/host
UID != id -u
GID != id -g
DK_SHARE_DIR ?= $(PWD)
DK_RUN_CMD = docker run \
-ti \
-u $(UID):$(GID) \
--rm \
-v $(DK_SHARE_DIR):$(DK_MNT_DIR) \
-w $(DK_MNT_DIR) \
$(DK_IMAGE_NAME)

build: .image-built 

.image-built:$(DK_DOCKERFILE_DIR)/Dockerfile
	docker image build -t $(DK_IMAGE_NAME) $(DK_DOCKERFILE_DIR)
	touch .image-built

dl: .image-built
pull: .image-built
	docker pull $(DK_IMAGE_NAME):latest
	touch .image-built

bash:.image-built
	$(DK_RUN_CMD) /bin/bash

exec:.image-built
	$(DK_RUN_CMD) $(CMD)

clean:
	docker image rm $(DK_IMAGE_NAME); \
	rm .image-built
