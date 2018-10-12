# OSON : Operating System Or Not

# Simulation avec QEMU
## Installation de RISCV-QEMU
### Installation par les sources

Building the full system emulator and the user mode emulator on Linux:
    apt update && apt install \
    build-essential zlib1g-dev pkg-config flex bison libglib2.0-dev binutils-dev libboost-all-dev autoconf libtool libssl-dev libpixman-1-dev libpython-dev python-pip python-capstone virtualenv
    git clone --recursive https://github.com/riscv/riscv-qemu.git
    cd riscv-qemu
    mkdir build
    cd build
    ../configure --target-list=riscv64-softmmu,riscv32-softmmu,riscv64-linux-user,riscv32-linux-user --prefix=$HOME/bin-qemu
    make -j$(nproc)
    make install




