#include "clock.h"

#include "encoding.h"
#include "csr.h"

void init_machine_clock() {
	csr_set(mstatus, MSTATUS_MIE);
	csr_set(mie, MIP_MTIP);

	set_mtimecmp(100000);
}
