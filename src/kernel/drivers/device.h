// See LICENSE for license details.

#pragma once


typedef struct console_device {
    void (*init)();
    int (*getchar)();
    int (*putchar)(int);
} console_device_t;

typedef struct poweroff_device {
    void (*init)();
    void (*poweroff)(int);
} poweroff_device_t;

typedef struct clint_device {
	int clk_freq;
	int base_addr;
} clint_device_t;

typedef struct plic_device {
	void (*init)();
} plic_device_t;

void register_console(console_device_t *dev);
void register_poweroff(poweroff_device_t *dev);
void register_clint(clint_device_t *dev);
void register_plic(plic_device_t *dev);

extern console_device_t *console_dev;
extern poweroff_device_t *poweroff_dev;
extern clint_device_t *clint_dev;
extern plic_device_t *plic_dev;

extern console_device_t console_none;
extern console_device_t console_htif;
extern console_device_t console_ns16550a;
extern console_device_t console_sifive_uart;
extern console_device_t console_cep_uart;
extern console_device_t console_semihost;

extern poweroff_device_t poweroff_none;
extern poweroff_device_t poweroff_htif;
extern poweroff_device_t poweroff_sifive_test;
extern poweroff_device_t cep_poweroff;
extern poweroff_device_t poweroff_semihost;

extern clint_device_t clint_none;
extern clint_device_t spike_clint;
extern clint_device_t sifive_clint;
extern clint_device_t cep_clint;

extern plic_device_t plic_none;
extern plic_device_t cep_plic;
extern plic_device_t sifive_plic;
