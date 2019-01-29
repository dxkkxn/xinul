#include "timer.h"

#include "csr.h"
#include "encoding.h"

void setup_clock_interrupts()
{
	csr_set(sie, MIP_STIP);

	// Schedule the first interruption in 100ms
	set_mtimecmp(get_mtime() + 100 * (SPIKE_CLOCK_FREQUENCY / 1000));
}
