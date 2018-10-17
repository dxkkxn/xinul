FROM debian:testing

RUN dpkg --add-architecture riscv64
RUN apt-get update && apt-get install -y \
gcc-riscv64-linux-gnu qemu-system-misc
