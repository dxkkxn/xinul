#!/bin/bash
#set -x
set -e

TOP=$PWD
S2B="python3 $TOP/src2base.py"
DEST_DIR=$(realpath base)
echo Base directory: $DEST_DIR
SRC_DIR=$DEST_DIR/src
USER_DIR=$SRC_DIR/user
KERNEL_DIR=$SRC_DIR/kernel

#rm -rf $DEST_DIR
mkdir -p $SRC_DIR

#
# Top dir
#
cp -r .gitignore README.md docker docs examples $DEST_DIR

#
# src
#
cd $TOP/src
cp Makefile gdbinit README.md riscv_flags.config toolchain.config $SRC_DIR

#
# std lib
#
cp -r $TOP/src/lib $SRC_DIR

#
# user land
#

cd $TOP/src/user
mkdir -p $USER_DIR
$S2B -o $USER_DIR Makefile
cp -r build tests hello $USER_DIR/
cd ulib
$S2B -o $USER_DIR/ulib crt0.c sbrk.c syscall.h
cp weak-syscall-stubs.S_ $USER_DIR/ulib/weak-syscall-stubs.S


#
# Kernel land
#

cd $TOP/src/kernel
# Build
$S2B -o $KERNEL_DIR Makefile `find build -type f`
# boot
$S2B -o $KERNEL_DIR `find boot -type f`
# BIOS
$S2B -o $KERNEL_DIR `find bios -type f`
# Drivers
$S2B -o $KERNEL_DIR `find drivers -type f`
# traps
cd traps
$S2B -o $KERNEL_DIR/traps machine_trap_entry.S machine_trap.c trap.h blue_screen.c
cd ..
# kernel
$S2B -o $KERNEL_DIR start.c mem.c timer.c timer.h userspace_apps.c userspace_apps.h empty.c cons_write.c cons_write.h

# GIT
cd $DEST_DIR
git init
git add * .gitignore
git commit -m "Xinul skeleton"

