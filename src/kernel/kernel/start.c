

#include "stdio.h"
#include "csr.h"

#include "process.h"
#include "program.h"

void idle()
{
	printf("Je suis idle\n");
	while(1);
}

int main(int argc, char **argv)
{
	printf("OSON Initialization.\n");
	
	init_process();
	
	unsigned int reg;
	volatile unsigned long* mtime =	  (unsigned long*)(0x2000000 + 0xbff8);
	volatile unsigned long* timecmp = (unsigned long*)(0x2000000 + 0x4000);
	


	// enable machine timer interrupt (MTIP field)
	csr_write(mie, 0x80);
	// enable machine interruption (MIE field)
	csr_write(mstatus, 0x08);

	*timecmp = *mtime + (10000);
	printf("mtime=%d\n", *mtime);
	reg = csr_read(mcycle);
	printf("reg: %x\n", reg);
	reg = csr_read(mcycle);
	printf("reg: %x\n", reg);
	printf("mtime=%d\n", *mtime);
	reg = csr_read(mip);
	printf("mip=%d\n", reg);
	printf("get_mtime: %d\n", get_mtime());
	while(1) {
			printf("get_mtime: %d\n", get_mtime());
			__asm__("wfi");
			//printf("mip=%d, mie=%d, mstatus=%d\n", csr_read(mip), csr_read(mie), csr_read(mstatus));
	}
	// Malloc does not work.
	//create_kernel_process(hello, "Hello", 100, 0);
	idle();

	return 0;
}
