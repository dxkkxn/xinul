FROM debian:testing

# Install Qemu from RISCV-QEMU source
## Make src directory
RUN mkdir -p /riscv/riscv-src && cd /riscv/riscv-src && \
## Install dependencies
apt-get update && apt-get install -y --no-install-recommends --no-install-suggests \
git build-essential zlib1g-dev pkg-config flex bison libglib2.0-dev binutils-dev libboost-all-dev autoconf libtool libssl-dev libpixman-1-dev libpython-dev python-pip python-capstone virtualenv && \
git clone --recursive https://github.com/riscv/riscv-qemu.git && \
cd riscv-qemu && mkdir build && cd build \
&& ../configure --target-list=riscv64-softmmu,riscv32-softmmu,riscv64-linux-user,riscv32-linux-user \
&& make && make install

# Build and install RISCV-tools
## Install riscv-fesvr
ENV RISCV=/tools
RUN apt-get install -y \
autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev libusb-1.0-0-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev device-tree-compiler pkg-config libexpat-dev && \
mkdir /tools && cd /riscv/riscv-src && cd /riscv/riscv-src && \
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