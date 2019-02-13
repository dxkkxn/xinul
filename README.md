# OSON : Operating System ... Or Not!


# Définition des variables d’environnement

À mettre dans `~/.bashrc` si vous utilisez `bash` ou dans votre `~/.zshrc` si vous utilisez `zsh`.
Ou a taper chaque fois que vous ouvrez un shell si vous aimer vous embêter !

```sh
$ export RISCV=/opt/riscv
$ export PATH=$RISCV/bin:$PATH
```

# Environnement de développement croisé
## Installation de GCC
### Installation à partir des sources
#### Dépendances :

```sh
$ sudo apt-get update
$ sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev
```

#### Compilation et installation

```sh
$ git clone --recurse-submodules https://github.com/riscv/riscv-gnu-toolchain
$ cd riscv-gnu-toolchain/  && mkdir build && cd build
$ ../configure --prefix=$RISCV
$ make
```

# Simulation avec QEMU
## Installation de RISCV-QEMU
### Installation à partir des sources

#### Dépendances :

```sh
$ sudo apt-get update
$ sudo apt-get install -y git python libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev libsdl1.2-dev
```

#### Compilation et installation

```sh
$ git clone https://github.com/riscv/riscv-qemu/
$ cd riscv-qemu && mkdir build && cd build
$ ../configure --prefix=$RISCV --target-list=riscv64-softmmu
$ make && make install
```

# Glossaire
## ISA

??

## Spike

* Spike, is the RISC-V ISA Simulator, implements a functional model of one or more RISC-V processors.
* Source du projet sur [github riscv/riscv-isa-sim](https://github.com/riscv/riscv-isa-sim).
