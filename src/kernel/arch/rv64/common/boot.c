// See LICENSE for license details.

#include "arch/riscv/encoding.h"
#include "machine.h"

int main(int argc, char **argv);

__attribute__((noreturn)) void boot_riscv()
{
	char *argv[] = { "femto", NULL };
	arch_setup();
	exit(main(1, argv));
	__builtin_unreachable();
}
