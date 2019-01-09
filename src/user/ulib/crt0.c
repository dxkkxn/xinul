/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 *
 * Bootstrap function linked with every application and used as entry point
 * in the app.
 */

extern int main(void*);
extern void proc_exit_user(void);
// extern void* get_arg(void);

/* Tell the compiler the function is in a special section */
void _start(void) __attribute__((section(".text.init")));

void _start(void) {
	//void* arg = get_arg();
	int tmp_var = 42;
	void* arg = &tmp_var;
        (void) main(arg);
        (void) proc_exit_user();
}
