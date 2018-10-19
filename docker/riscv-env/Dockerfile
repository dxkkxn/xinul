FROM debian:testing

# Install X-compile and QEMU.
RUN dpkg --add-architecture riscv64 && \
apt-get update && apt-get install -y \
gcc-riscv64-linux-gnu qemu-system-misc

# Build and install RISCV-tools
ENV RISCV=/tools
## Install riscv-fesvr
RUN apt-get install -y \
git autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev libusb-1.0-0-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev device-tree-compiler pkg-config libexpat-dev && \
mkdir /tools && mkdir -p riscv/riscv-src && cd /riscv/riscv-src && cd /riscv/riscv-src && \
git clone https://github.com/riscv/riscv-fesvr && \
cd riscv-fesvr && mkdir build && cd build && \
../configure --prefix=$RISCV --target=riscv64-unknown-elf && \
make install

## Spike
RUN cd /riscv/riscv-src && \
git clone https://github.com/riscv/riscv-isa-sim && \
echo ok
RUN cd /riscv/riscv-src/riscv-isa-sim && mkdir build && cd build && \
../configure --prefix=$RISCV --with-fesvr=$RISCV && \
make install
ENV PATH=$PATH:/tools/bin/