FROM debian:testing

RUN dpkg --add-architecture riscv64
RUN apt update && apt install -y build-essential gcc-riscv64-linux-gnu g++-riscv64-linux-gnu qemu-system-misc
