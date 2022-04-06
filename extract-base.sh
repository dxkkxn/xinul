#!/bin/bash
#set -x
set -e

TOP=$PWD/extract_tmp
S2B="python3 $TOP/src2base.py"
DEST_DIR=$(realpath xinul)
SRC_DIR=$DEST_DIR/src
USER_DIR=$SRC_DIR/user
KERNEL_DIR=$SRC_DIR/kernel

# Check if output and TOP directories exist
if [ -d $TOP ]; then
  echo "Error - The directory $TOP already exist. Press y to remove."
  read input
  if [[ $input = y ]]; then
    echo "Remove $TOP"
    rm -rf $TOP
else
  exit 1
fi
fi

if [ -d $DEST_DIR ]; then
  echo "Error - The directory $DEST_DIR already exist. Press y to remove."
  read input
  if [[ $input = y ]]; then
    echo "Remove $DEST_DIR"
    rm -rf $DEST_DIR
else
  exit 1
fi
fi

# Archive repo to avoid additional files copy.
mkdir $TOP
git archive --format=tar HEAD | \
    tar x -C $TOP

mkdir -p $SRC_DIR

#
# Top dir
#
cd $TOP
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
# Tests
cd tests
$S2B -o $KERNEL_DIR/tests it.c it.h
cd ..
# kernel
$S2B -o $KERNEL_DIR start.c mem.c timer.c timer.h userspace_apps.c userspace_apps.h empty.c cons_write.c cons_write.h

# GIT
cd $DEST_DIR
git init
git add * .gitignore
git commit -m "Xinul skeleton"

