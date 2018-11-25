#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"

#include "process.h"
#include "program.h"
#include "clock.h"

#include "csr.h"
#include "stdint.h"


void extern ctx_sw(struct cpu_state *, struct cpu_state *);

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

void enter_supervisor_mode()
{
	uint64_t pmpcfg = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
	uint64_t pmpaddr = ((uint64_t)1U << 53)-1;
	csr_set(mstatus, MSTATUS_MPP_MASK_S);
	csr_clear(mstatus, MSTATUS_MPIE_MASK);
	csr_write(pmpaddr0, pmpaddr);
	csr_write(pmpcfg0, pmpcfg);

	csr_write(mepc, idle);
	__asm__ __volatile__ ("mret");
}


int main(int argc, char **argv)
{
	printf("\n= OSON Initialization =\n");
	init_process();
	//init_machine_clock();

	printf("mstatus: 0x%x\n", csr_read(mstatus));
	printf("mepc: 0x%x\n", csr_read(mepc));
	
	enter_supervisor_mode();
	printf("mstatus: 0x%x\n", csr_read(mstatus));
	if ( (create_kernel_process(hello, "Hello", 100, (void*) 42)) == NULL)
	{
		printf("Process error: unable to create process hello. exit.\n");
		exit(-1);
	}
	idle();

	return 0;
}
