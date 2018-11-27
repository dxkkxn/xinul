#include "handlers.h"
#include "csr.h"

void handle_mtimer_interrupt() {
	set_mtimecmp(get_mtime() + 1000000);
}
