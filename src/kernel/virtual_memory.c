/** See LICENSE for license details.
* virtual_memory.c
*
*  Created on 1 décembre 2018
*/

#include "stdio.h"
#include "stdlib.h"

#include "machine.h"
#include "virtual_memory.h"

// kernel_satp_csr to store kernel pagetable root used by all kernel process.
static satp_csr kernel_satp;
static pagetable_t kernel_root_pagetable = NULL;
static struct pte kernel_gigapage;

/* init virtual memory
* returns -1 if a error occurred.
*/
int8_t init_virtual_memory()
{
 printf("## Init virtual memory\n");
 
 kernel_root_pagetable = memalign(PAGE_SIZE, PAGE_SIZE);
 if (kernel_root_pagetable == NULL) return -1;
 
 kernel_gigapage.V = 1;
 kernel_gigapage.R = 1;
 kernel_gigapage.W = 1;
 kernel_gigapage.X = 1;
 kernel_gigapage.U = 0;
 kernel_gigapage.G = 1;
 kernel_gigapage.A = 1;
 kernel_gigapage.D = 1;
 kernel_gigapage.RSW = 0;
 kernel_gigapage.PPN0 = 0;
 kernel_gigapage.PPN1 = 0;
 kernel_gigapage.PPN2 = 2;
 kernel_gigapage.RSSV = 0;
 kernel_root_pagetable[2] = kernel_gigapage;

 // Configure satp csr 
 kernel_satp.field.MODE = SATP_MODE_SV39;
 kernel_satp.field.ASID = 0;
 kernel_satp.field.PPN = (uint64_t)kernel_root_pagetable >> 12;

 write_csr(satp, kernel_satp.reg);
 
 return 0;
}

satp_csr get_kernel_satp()
{
 return kernel_satp;
}

satp_csr init_user_virtual_memory(uint16_t asid)
{
 
 pagetable_t root_pagetable = memalign(PAGE_SIZE, PAGE_SIZE);
 root_pagetable[2] = kernel_gigapage;
 satp_csr satp;
 satp.field.MODE = SATP_MODE_SV39;
 satp.field.ASID = asid;
 satp.field.PPN = (uint64_t)root_pagetable >> 12;
 return satp;
}

pagetable_t get_root_pagetable(satp_csr satp)
{
 return (pagetable_t)( (uint64_t)satp.field.PPN << 12);
}

void free_user_virtual_memory(satp_csr satp)
{
 pagetable_t root_pagetable = get_root_pagetable(satp);
 free((void*) root_pagetable);
}
