/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 *
 * Bootstrap function linked with every application and used as entry point
 * in the app.
 */

#include "syscall.h"

extern int main(void *);

/* Tell the compiler the function is in a special section */
void _start(void *arg) __attribute__((section(".text.init")));

void _start(void *arg)
{
	exit(main(arg));
}
