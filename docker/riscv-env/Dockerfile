FROM debian:testing

RUN dpkg --add-architecture riscv64
RUN apt update && apt install -y build-essential gcc-riscv64-linux-gnu g++-riscv64-linux-gnu
RUN cat > /etc/ld.so.conf.d/riscv64-linux-gnu.conf << \
/usr/local/lib/riscv64-linux-gnu\n\
/lib/riscv64-linux-gnu\r\
/usr/lib/riscv64-linux-gnu\r\
/usr/riscv64-linux-gnu/lib/\r\
EOF
# RUN ln -s /usr/riscv64-linux-gnu/lib/ld-linux-riscv64-lp64d.so.1 / lib
