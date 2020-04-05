#!/bin/bash
#set -x
set -e

TOP=$PWD
S2B="python3 $TOP/src2base.py"
BASE_DIR=$(realpath base)
echo Base directory: $BASE_DIR
SRC_DIR=$BASE_DIR/src
USER_DIR=$SRC_DIR/user
KERNEL_DIR=$SRC_DIR/kernel

rm -rf $BASE_DIR
mkdir -p $SRC_DIR

cp -r README.md docker $BASE_DIR

#
# src
#
cd $TOP/src
cp Makefile .gitignore gdbinit README.md riscv_flags.config toolchain.config $SRC_DIR

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

cd $TOP/src/kernel/boot
$S2B -o $KERNEL_DIR/boot `find common -type f`
