FROM debian:testing

LABEL maintainer="Mathieu Barbe <mathieu@kolabnow.com>"
LABEL Description="This image is used to cross compile and run Qemu for RISC-V project"

# Set RISC-V env
ENV RISCV=/tools
ENV PATH=$PATH:/tools/bin/

# Install dependencies
RUN apt-get update && apt-get install -y --no-install-recommends --no-install-suggests git ca-certificates autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev && \
apt-get clean && \

# Make src and tools directory
mkdir /tools /riscv-src/ && cd /riscv-src && \

# Build and install RISCV-tools into /tools directory
## Install gnu-toolchain
cd /riscv-src && \
git clone --recurse-submodules -j8 https://github.com/riscv/riscv-gnu-toolchain && \
cd /riscv-src/riscv-gnu-toolchain && mkdir build && cd build && \
../configure --prefix=$RISCV && \
make && \
rm -rf /riscv-src/riscv-gnu-toolchain

## Install riscv-fesvr
RUN cd /riscv-src && \
git clone https://github.com/riscv/riscv-fesvr && \
cd riscv-fesvr && mkdir build && cd build && \
../configure --prefix=$RISCV --target=riscv64-unknown-elf && \
make install && \
rm -rf /riscv-src/riscv-fesvr && \

## Spike
apt-get install -y device-tree-compiler && apt-get clean && \
cd /riscv-src && \
git clone https://github.com/riscv/riscv-isa-sim && \
cd riscv-isa-sim && mkdir build && cd build && \
../configure --prefix=$RISCV --with-fesvr=$RISCV && \
make install && \
rm -rf /riscv-src/riscv-isa-sim && \

## Install pk (Proxy Kernel)
cd /riscv-src && \
git clone https://github.com/riscv/riscv-pk && \
cd /riscv-src/riscv-pk && mkdir build && cd build && \
../configure --prefix=$RISCV --host=riscv64-unknown-elf && \
make install && \
rm -rf /riscv-src/riscv-pk

# Install Qemu from RISCV-QEMU source
RUN cd /riscv-src && \
apt-get update && apt-get install -y git python libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev libsdl1.2-dev && apt-get clean && \
git clone https://github.com/riscv/riscv-qemu/ && \
cd riscv-qemu && mkdir build && cd build && \
../configure --prefix=$RISCV --target-list=riscv64-softmmu | tee /$(RISCV)/qemu-configure-log.txt && \ 
make && make install && \
rm -rf /riscv-src

EXPOSE 1234


