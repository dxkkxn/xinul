FROM debian

LABEL maintainer="Mathieu Barbe <mathieu@kolabnow.com>"
LABEL Description="This image is used to cross compile and run Qemu for RISC-V project"

# Set RISC-V env
ENV RISCV=/tools
ENV PATH=$PATH:$RISCV/bin/

#
# Build and install RISCV-tools into RISCV directory
#

# Install dependencies
RUN apt-get update && apt-get install -y --no-install-recommends --no-install-suggests git ca-certificates autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev && \
apt-get clean && \

# Make src and tools RSCV directory
mkdir -p $RISCV

## Download and Install gnu-toolchain
RUN mkdir /src && cd /src && \
git clone --recurse-submodules -j8 https://github.com/riscv/riscv-gnu-toolchain && \

mkdir /src/riscv-gnu-toolchain/build && cd /src/riscv-gnu-toolchain/build && \
../configure --prefix=$RISCV && \
make && \
rm -rf /src

#
# Install Qemu RISC-V CEP machine
RUN mkdir -p /src && cd /src && \
apt-get update && apt-get install -y git python libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev libsdl1.2-dev && apt-get clean && \
git clone https://github.com/raifer/qemu-cep.git --branch=riscv_cep_4.1.0 && \
mkdir -p /src/qemu-cep/build && cd /src/qemu-cep/build && \
../configure --prefix=$RISCV --target-list=riscv64-softmmu | tee $RISCV/qemu-configure-log.txt && \
make && make install && \
rm -rf /src

## Install riscv-fesvr
RUN mkdir -p /src && cd /src && \
git clone https://github.com/riscv/riscv-fesvr && \
mkdir -p /src/riscv-fesvr/build && cd /src/riscv-fesvr/build && \
../configure --prefix=$RISCV --target=riscv64-unknown-elf && \
make install && \
rm -rf /src


## Spike
RUN apt-get install -y device-tree-compiler && apt-get clean && \
mkdir -p /src && cd /src && \
git clone https://github.com/riscv/riscv-isa-sim && \
mkdir -p /src/riscv-isa-sim/build && cd /src/riscv-isa-sim/build && \
../configure --prefix=$RISCV --with-fesvr=$RISCV && \
make install && \
rm -rf /src

## Install pk (Proxy Kernel)
RUN mkdir -p /src && cd /src && \
git clone https://github.com/riscv/riscv-pk && \
mkdir -p /src/riscv-pk/build && cd /src/riscv-pk/build && \
../configure --prefix=$RISCV --host=riscv64-unknown-elf && \
make install && \
rm -rf /src

EXPOSE 1234


