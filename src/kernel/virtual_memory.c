/** See LICENSE for license details.
* virtual_memory.c
*
*  Created on 1 décembre 2018
*/

#include "stdio.h"
#include "stdlib.h"

#include "machine.h"
#include "virtual_memory.h"

/* init virtual memory
* returns -1 if a error occurred.
*/
int8_t init_virtual_memory()
{
 printf("## Init virtual memory\n");
 
 pagetable_t root_pt = memalign(PAGE_SIZE, PAGE_SIZE);
 struct pte * gigapage = &(root_pt[2]);
 gigapage->V = 1;
 gigapage->R = 1;
 gigapage->W = 1;
 gigapage->X = 1;
 gigapage->U = 0;
 gigapage->G = 1;
 gigapage->RSW = 0;
 gigapage->PPN0 = 2;
 gigapage->PPN1 = 0;
 gigapage->PPN2 = 0;
 gigapage->RSSV = 0;
 
 struct satp_csr satp;
 satp.MODE = SATP_MODE_SV39;
 satp.ASID = 0;
 satp.PPN = (uint64_t)root_pt >> 12;

 //write_csr(satp, satp);
 
 return 0;
}
