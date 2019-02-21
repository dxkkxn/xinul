/*
 * Projet PCSEA RISC-V
 *
 * Mathieu Barbe <mathieu@kolabnow.com> - 2019
 *
 * See license for license details.
 */

#include "stdio.h"
#include "stdlib.h"
#include "riscv.h"

#include "trap.h"

static const char *land_names[4] = {
		"User",
		"Supervisor",
		"Hypervisor",
		"Machine"
};

static const char *exception_names[16] = {
		"misaligned_fetch",
		"fault_instruction_fetch",
		"illegal_instruction",
		"breakpoint",
		"misaligned_load",
		"fault_load",
		"misaligned_store",
		"fault_store",
		"user_ecall",
		"supervisor_ecall",
		"hypervisor_ecall",
		"machine_ecall",
		"exec_page_fault",
		"load_page_fault",
		"reserved",
		"store_page_fault"
};

static const char *regnames[] = {
		"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
		"s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
		"a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
		"s8", "s9", "sA", "sB", "t3", "t4", "t5", "t6"
};

static void dump_mtrap_frame(struct trap_frame *tf)
{
	puts("GPR :");
	uint64_t *regs = (uint64_t *) tf;
	for (int i = 0; i < 32; i += 4) {
		for (int j = 0; j < 4; j++)
			printf("%s %lx%c", regnames[i + j], regs[i + j], j < 3 ? ' ' : '\n');
	}

	puts("\nException cause :");
	printf("pc %lx\ttval %lx\n", tf->mepc, tf->mtval);

	puts("\nCSR :");
	printf("status %lx\n", tf->mstatus);
	printf("satp %lx\n", tf->satp);
}

void blue_screen(struct trap_frame *tf)
{
	printf("\n********  RISC-V Exception N°%ld : %s  ********\n\n", tf->mcause, exception_names[tf->mcause]);

	dump_mtrap_frame(tf);

	uint8_t land = (uint8_t)((tf->mstatus & MSTATUS_MPP) >> 11);
	printf("\n%s from %s land at %lx\n", exception_names[tf->mcause], land_names[land], tf->mepc);
	exit(-tf->mcause);
}


