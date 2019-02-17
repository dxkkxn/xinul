#
# Docker RISC-V Makefile
# 
# Mathieu Barbe <mathieu.barbe@grenoble-inp.org> - 2018 - 2019
# See LICENSE for license details.
#

IMAGE_NAME = raifer/risc-v
DOCKERFILE_DIR = .
DK_MNT_DIR = /mnt/host
UID != id -u
GID != id -g
DK_SHARE_DIR = $(PWD)
RUN_CMD = docker run
RUN_OPTS = \
-ti \
-u $(UID):$(GID) \
--rm \
-v $(DK_SHARE_DIR):$(DK_MNT_DIR) \
-w $(DK_MNT_DIR)

build: .image-built 

.image-built: $(DOCKERFILE_DIR)/Dockerfile Makefile
	docker image build -t $(IMAGE_NAME) $(DOCKERFILE_DIR)
	touch .image-built

.PHONY: dl, pull
dl pull : 
	docker pull $(IMAGE_NAME):latest
	touch .image-built

bash:.image-built
	$(RUN_CMD) $(RUN_OPTS) $(IMAGE_NAME) /bin/bash

# Exécuter une commande avec un container
# DK_OPTS et CMD défnie par l'appelant
exec:.image-built
	$(RUN_CMD) $(RUN_OPTS) $(DK_OPTS) $(IMAGE_NAME) $(CMD)

clean:
	docker image rm $(IMAGE_NAME); \
	rm -f .image-built
