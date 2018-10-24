FROM debian:testing

# Install Qemu from RISCV-QEMU source
## Make src directory
RUN mkdir -p /riscv-src/ && cd /riscv-src && \
## Install dependencies
apt-get update && apt-get install -y --no-install-recommends --no-install-suggests \
autoconf automake autotools-dev bc binutils-dev bison build-essential curl device-tree-compiler flex gawk git gperf libboost-all-dev libexpat-dev libglib2.0-dev libgmp-dev libmpc-dev libmpfr-dev libpixman-1-dev libpython-dev libtool libtool libssl-dev libusb-1.0-0-dev patchutils pkg-config python-capstone python-pip texinfo virtualenv zlib1g-dev && \
git clone --recursive https://github.com/qemu/qemu.git && \
cd qemu && mkdir build && cd build && \
../configure --target-list=riscv64-softmmu,riscv64-linux-user && \
make && make install

# Build and install RISCV-tools into /tools directory
ENV RISCV=/tools
ENV PATH=$PATH:/tools/bin/
## Install riscv-fesvr
RUN mkdir /tools && cd /riscv-src && cd /riscv-src && \
git clone https://github.com/riscv/riscv-fesvr && \
cd riscv-fesvr && mkdir build && cd build && \
../configure --prefix=$RISCV --target=riscv64-unknown-elf && \
make install

## Spike
RUN cd /riscv-src && \
git clone https://github.com/riscv/riscv-isa-sim && \
cd riscv-isa-sim && mkdir build && cd build && \
../configure --prefix=$RISCV --with-fesvr=$RISCV && \
make install

## Install gny-toolchain
RUN cd /riscv-src && \
git clone --recurse-submodules  https://github.com/riscv/riscv-gnu-toolchain
RUN cd /riscv-src/riscv-gnu-toolchain && mkdir build && cd build && \
../configure --prefix=$RISCV && \
make

## Install pk (Proxy Kernel)
RUN cd /riscv-src && \
git clone https://github.com/riscv/riscv-pk
RUN cd /riscv-src/riscv-pk && mkdir build && cd build && \
../configure --prefix=$RISCV --host=riscv64-unknown-elf && \
make install

