#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"

#include "process.h"
#include "program.h"
#include "clock.h"

#include  "machine.h"
#include "csr.h"
#include "stdint.h"
#include "trap.h"
#include "irq.h"
#include "sbi.h"


void extern ctx_sw(struct cpu_state *, struct cpu_state *);
void extern strap_entry();

#define MSTATUS_MPP_MASK_S 0x800 /* bit 12-11 = 01 */
#define MSTATUS_MPIE_MASK 0x80 /* bit 7 = 1 */
#define MSTATUS_SPIE_MASK 0x20 /* bit 5 = 1 */


void initialize_mstatus()
{
	// Enable machine software interrupt
	csr_write(mie, MIX_MSI);
}

int set_next_timer_event()
{
	uint64_t delta = 1000000;
	// set the new mtimecmp value and activate the machine timer interruptions
	sbi_call_set_timer(delta);
}

void strap_handler(uintptr_t* regs, uintptr_t scause, uintptr_t sepc)
{
	if (scause & INTERRUPT_CAUSE_FLAG) {
		switch (scause & ~INTERRUPT_CAUSE_FLAG) {
		case intr_s_timer:
			// Set a new timer interrupt
			set_next_timer_event();
			// Clear the interrupt flag so that the processor does not take
			// this trap again after return from interrupt
			csr_clear(sip, MIX_STI);
			break;
		default:
			die("supervisor mode: unhandable interrupt %d @ %p", scause, sepc);
			break;
		}
	} else {
		die("supervisor mode: unhandable exception %d @ %p", scause, sepc);
	}
}

void delegate_traps()
{
		// Delegate all the supervisor interruption to the supervisor
		uint64_t interrupts = MIX_SSI | MIX_STI | MIX_SEI;
		// Delegate most of the exceptions to the supervisor
		uint64_t exceptions =
			(1U << cause_instruction_address_misaligned)	|
			(1U << cause_breakpoint)						|
			(1U << cause_illegal_instruction)				|
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

	// allow access to all of the memory for everyone
	csr_write(pmpaddr0, pmpaddr);
	csr_write(pmpcfg0, pmpcfg);

	// Enable supervisor use of counters
	csr_write(scounteren, -1);

	// Disable paging (bare memory)
	csr_write(satp, 0);

	// set the previous context in mstatus
	csr_set(mstatus, MSTATUS_MPP_MASK_S);

	// Set the trap vector (direct mode) and enable interrupts for the
	// supervisor mode
	csr_write(stvec, (unsigned long)strap_entry | 0UL);
	csr_set(mstatus, MSTATUS_SIE);
	// Enable timer interrupt
	csr_set(mie, MIX_STI);

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
	ctx_sw(&idle->cpu_state, &hello->cpu_state);
	printf("Encore une fois dans le idle\n");
	ctx_sw(&idle->cpu_state, &hello->cpu_state);
	
	printf("On entre dans la boucle infinie du idle\n");
	unsigned int reg, i;
	i=0;
	while(i < 10) {
		printf("timer interrupt n°%d\n", i);
		__asm__("wfi");
		i++;
	}
	printf("On sort de la boucle infinie pour éviter de faire un make kill\n");
}

int main(int argc, char **argv)
{
	printf("\n= OSON Initialization =\n");
	initialize_mstatus();

	init_machine_clock();

	delegate_traps();
	enter_supervisor_mode();
	
	init_process();

	//csr_read(mstatus);


	if ( (create_kernel_process(hello, "Hello", 100, (void*) 42)) == NULL)
	{
		printf("Process error: unable to create process hello. exit.\n");
		exit(-1);
	}
	idle();

	return 0;
}
