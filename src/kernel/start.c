#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdint.h"

#include "process.h"
#include "program.h"
#include "clock.h"
#include "virtual_memory.h"
#include "csr.h"
#include "trap.h"
#include "irq.h"

void ctx_sw(struct context *, struct context *);

#define MSTATUS_MPP_MASK_S 0x800 /* bit 12-11 = 01 */
#define MSTATUS_MPIE_MASK 0x80 /* bit 7 = 1 */

#define PMP_NAPOT 0x18
#define PMP_R 0x1
#define PMP_W 0x2
#define PMP_X 0x4
#define PMP_A 0x18
#define PMP_L 0x80
#define PMP_SHIFT 0x2
#define PMP_TOR 0x8
#define PMP_NA4 0x10


void initialize_mstatus()
{
	// potentially enable FPU, potentially enable user use of counters

	// Enable supervisor use of counters
	csr_write(scounteren, -1);

	// Enable software interrupt
	csr_write(mie, MIX_MSI);

	// Disable paging (bare memory)
	csr_write(satp, 0);
}

void delegate_traps()
{
		// Delegate all the supervisor interruption to the supervisor
		uint64_t interrupts = MIX_SSI | MIX_STI | MIX_SEI;
		// Delegate most of the exceptions to the supervisor
		uint64_t exceptions =
			(1U << cause_instruction_address_misaligned)	|
			(1U << cause_breakpoint)						|
			(1U << cause_instruction_page_fault)			|
			(1U << cause_load_page_fault)					|
			(1U << cause_store_page_fault)					|
			(1U << cause_user_ecall);
		
		csr_write(mideleg, interrupts);
		csr_write(medeleg, exceptions);
}

void enter_supervisor_mode()
{
	uint64_t pmpcfg = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
	uint64_t pmpaddr = ((uint64_t)1U << 53)-1;
	csr_set(mstatus, MSTATUS_MPP_MASK_S);
	csr_clear(mstatus, MSTATUS_MPIE_MASK);
	csr_write(pmpaddr0, pmpaddr);
	csr_write(pmpcfg0, pmpcfg);

	__asm__ __volatile__ (
			"la t0, 1f\n"
			"csrw mepc, t0\n"
			"mret\n"
			"1:" ::: "t0"
	);
}


void idle()
{
	printf("Je suis idle\n");
	process_t idle = get_process(0);
	process_t hello = get_process(1);
	process_t hello_user = get_process(2);
	ctx_sw(&idle->context, &hello->context);
	printf("Encore une fois dans le idle\n");
	ctx_sw(&idle->context, &hello->context);
	printf("on refait un tour dans idle et on lance hello_user\n");
	ctx_sw(&idle->context, &hello_user->context);
	
	printf("On entre dans la boucle infinie du idle\n");
	unsigned int i = 0;
	while(i < 10) {
		printf("timer interrupt n°%d\n", i);
		__asm__("wfi");
		i++;
	}
	printf("On sort de la boucle infinie pour éviter de faire un make kill\n");
}

int main()
{
	printf("\n# OSON Initialization #\n");
	initialize_mstatus();

	init_machine_clock();

	delegate_traps();
	enter_supervisor_mode();
	
	init_process();
	
	init_virtual_memory();


	if ( (create_kernel_process(hello, "Hello", 100, (void*) 42)) == NULL)
	{
		printf("Process error: unable to create process hello. exit.\n");
		exit(-1);
	}
	
	process_t hello_user_process = create_user_process("hello_user", "Hello_user", 100, 0, (void*) 42);
	if (hello_user_process == NULL)
		{
			printf("Process error: unable to create user process hello_user. exit.\n");
			exit(-1);
		}
		
	idle();

	return 0;
}

