FROM debian


# Install Qemu from RISCV-QEMU source

RUN mkdir -p /riscv/riscv-src && cd /riscv/riscv-src && \
## Install dependencies
apt update && apt install -y --no-install-recommends --no-install-suggests \
git build-essential zlib1g-dev pkg-config flex bison libglib2.0-dev binutils-dev libboost-all-dev autoconf libtool libssl-dev libpixman-1-dev libpython-dev python-pip python-capstone virtualenv && \
git clone --recursive https://github.com/riscv/riscv-qemu.git && \
cd riscv-qemu && mkdir build && cd build \
&& ../configure --target-list=riscv64-softmmu,riscv32-softmmu,riscv64-linux-user,riscv32-linux-user \
&& make && make install

# Build and install RISCV-tools
RUN mkdir /tools && export RISCV=/tools && \
apt install -y autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev libusb-1.0-0-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev device-tree-compiler pkg-config libexpat-dev
RUN cd /riscv/riscv-src && \
git clone --recurse-submodules https://github.com/riscv/riscv-tools