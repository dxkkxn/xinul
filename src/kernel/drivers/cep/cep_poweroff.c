// See LICENSE for license details.

#include "device.h"
#include "stdint.h"
#include "auxval.h"

#define CEP_VALUE_EXIT  0x3333

static volatile uint32_t *test;

static void cep_test_init()
{
	test = (uint32_t *) (void *) getauxval(CEP_POWEROFF_CTRL_ADDR);
}

static void cep_test_poweroff(int status)
{
	*test = (status << 16) | CEP_VALUE_EXIT;
	while (1);
}

poweroff_device_t cep_poweroff = {
		cep_test_init,
		cep_test_poweroff
};
