#include "sbi.h"

void sbi_call_set_timer(uint64_t value)
{
	SBI_CALL_1(SBI_SET_TIMER, value);
}
