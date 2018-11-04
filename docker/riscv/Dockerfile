FROM debian:testing

MAINTAINER Mathieu Barbe <mathieu@kolabnow.com>

# Set RISC-V env
ENV RISCV=/tools
ENV PATH=$PATH:/tools/bin/

# Install dependencies
RUN apt-get update && apt-get install -y --no-install-recommends --no-install-suggests git ca-certificates autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev && \
apt-get clean && \

# Make src and tools directory
mkdir /tools /riscv-src/ && cd /riscv-src && \

# Build and install RISCV-tools into /tools directory
## Install gny-toolchain
cd /riscv-src && \
git clone --recurse-submodules -j8 https://github.com/riscv/riscv-gnu-toolchain && \
cd /riscv-src/riscv-gnu-toolchain && mkdir build && cd build && \
../configure --prefix=$RISCV && \
make && \
cd / && rm -rf /riscv-src/riscv-gnu-toolchain

## Install riscv-fesvr
RUN cd /riscv-src && cd /riscv-src && \
git clone https://github.com/riscv/riscv-fesvr && \
cd riscv-fesvr && mkdir build && cd build && \
../configure --prefix=$RISCV --target=riscv64-unknown-elf && \
make install && \
cd / && rm -rf /riscv-src/riscv-fesvr

## Spike
RUN apt-get install -y device-tree-compiler && apt-get clean && \
cd /riscv-src && \
git clone https://github.com/riscv/riscv-isa-sim && \
cd riscv-isa-sim && mkdir build && cd build && \
../configure --prefix=$RISCV --with-fesvr=$RISCV && \
make install && \
cd / && rm -rf /riscv-src/riscv-isa-sim && \

## Install pk (Proxy Kernel)
cd /riscv-src && \
git clone https://github.com/riscv/riscv-pk && \
cd /riscv-src/riscv-pk && mkdir build && cd build && \
../configure --prefix=$RISCV --host=riscv64-unknown-elf && \
make install && \
cd / && rm -rf /riscv-src/riscv-pk

# Install Qemu from RISCV-QEMU source
RUN apt-get update && apt-get install -y git python libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev libsdl1.2-dev && \
git clone https://github.com/riscv/riscv-qemu/ && \
cd riscv-qemu && mkdir build && cd build && \
../configure --prefix=$RISCV --target-list=riscv64-softmmu,riscv64-linux-user | tee /$(RISCV)/qemu-configure-log.txt && \ 
make && make install && \
apt-get clean && \
rm -rf /riscv-src 


