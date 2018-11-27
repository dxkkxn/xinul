#include "clock.h"
#include "irq.h"
#include "csr.h"

void init_machine_clock() {
	enable_machine_irq();
	enable_machine_timer_irq();

	set_mtimecmp(100000);
}
