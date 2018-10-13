FROM debian

# Install Qemu from RISCV-QEMU source
## Install dependencies
RUN apt update && apt install -y --no-install-recommends --no-install-suggests \
git build-essential zlib1g-dev pkg-config flex bison libglib2.0-dev binutils-dev libboost-all-dev autoconf libtool libssl-dev libpixman-1-dev libpython-dev python-pip python-capstone virtualenv && \
rm -rf /var/cache/apt && \
git clone --recursive https://github.com/riscv/riscv-qemu.git && \
cd riscv-qemu && mkdir build && cd build \
&& ../configure --target-list=riscv64-softmmu,riscv32-softmmu,riscv64-linux-user,riscv32-linux-user \
&& make && make install

# Fetch projects repositories from Gitlab.ensimag.fr
## Install ssh, 
### Copy SSH private key: You must paste your gitlab private RSA key into docker directory
COPY id_rsa /root/.ssh/id_rsa
COPY gitlab.pem /root/.ssh/known_hosts
RUN chmod 600 /root/.ssh/id_rsa && \
apt update && apt install -y openssh-client && rm -rf /var/cache/apt/ && \
## Clone repos
mkdir /sle && cd /sle && \
git clone git@gitlab.ensimag.fr:os_on_risc_v/oson.git && \
git clone git@gitlab.ensimag.fr:os_on_risc_v/documentations.git && \
git clone git@gitlab.ensimag.fr:os_on_risc_v/hardware.git

