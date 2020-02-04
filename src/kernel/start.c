/*
 * Projet PCSEA RISC-V
 *
 * Benoît Wallon <benoit.wallon@grenoble-inp.org> - 2019
 * Mathieu Barbe <mathieu@kolabnow.com> - 2019
 *
 * See license for license details.
 */

#include "stdio.h"
#include "assert.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"

#include "scheduler.h"
#include "program.h"
#include "tests.h"
#include "vmm.h"
#include "hmm.h"
#include "shm.h"
#include "syscall.h"
#include "timer.h"
#include "kbd.h"

// Programme par défaut en fonction de la machine, cf kernel/bot/$(MACHINE)/setup.c
extern char * default_program;

static char *splash[][2] = {
   {"\x1b[91m"," oooooo    oooooo        MMM         ooooo    oooooo                       oMMMMMM      "},
   {"\x1b[93m"," MMMMMM    MMMMMM        °°°         MMMMMo   MMMMMM                       °°°oMMM      "},
   {"\x1b[33m","   MMMM    MMM°                      °MMMMMo    °MMM                           MMM      "},
   {"\x1b[91m","    °MMMooMMM°       oMMMMMM          MMM°MMM    MMM  oMMMMM   oMMMMM          MMM      "},
   {"\x1b[95m","      °MMMM°          °°°MMM          MMM °MMMo  MMM   °°MMM    °°MMM          MMM      "},
   {"\x1b[94m","      MMMMMo             MMM          MMM   MMMo MMM     MMM      MMM          MMM      "},
   {"\x1b[91m","    oMMM°°MMMo           MMM          MMM    MMMMMMM     MMM      MMM          MMM      "},
   {"\x1b[33m","  oMMM°    °MMMo         MMM          MMM     °MMMMM     MMMo    oMMM          MMM      "},
   {"\x1b[93m","oMMMMMMo  oMMMMMMo  MMMMMMMMMMMMMM  MMMMMMMM   °MMMM     °MMMMMMMMMMMMM   MMMMMMMMMMMMMM"},
   {"\x1b[91m","YYYYYYYY  YYYYYYYY  YYYYYYYYYYYYYY  YYYYYYYY     YYY        YYYYY YYYY    YYYYYYYYYYYYYY"},
   {"\x1b[0m", "= Initializating XiNul, Ensimag' educational RISC-V proto-os ="}
};


static void splash_screen(void)
{
   for (int i = 0; i < 11; i++)
      printf("%s%s\n", splash[i][0], splash[i][1]);
}


int64_t launcher(void *arg)
{
	int pid;
	printf("Launcher start.\n");
	if ((pid = sched_ustart(default_program, 1024, 11, (void *) 0)) < 0) {
		assert(0 && "Unable to run default machine app\n");
	}
	return sched_waitpid(pid, NULL);
}

int64_t kernel_tests(void *arg)
{
	puts("Tests kernel");
	int pid = sched_kstart(test0, 10, "Test 0", 0);
	sched_waitpid(pid, NULL);
	pid = sched_kstart(test1, 10, "test1", 0);
	sched_waitpid(pid, NULL);
	puts("Kernel tests done.");
	return 0;
}

/* VGA fonts */
#include "font.h"
void render(char *bitmap, uint32_t x, uint32_t y, uint32_t color)
{
   uint32_t dx, dy;
   for (dy = 0; dy < 8; dy++) {
      for (dx = 0; dx < 8; dx++) {
         if (bitmap[dy] & 1 << dx)
            *(volatile uint32_t *)((uint32_t *)0x80000000 + (y + dy) * 1280 + x + dx) = color;
      }
   }
}

static void splash_vga_screen(void)
{
   for (int i = 0; i < 1280*768; i++)
      *(volatile uint32_t *)((uint32_t *)0x80000000 + i) = 0x00000000;

   uint32_t color[] = {0x00ff0000, 0x00ffff00, 0x00ffff00, 0x00ff0000, 0x00ff00ff, 0x000000ff, 0x00ff0000, 0x00ffff00, 0x00ffff00, 0x00ff0000, 0x0000ff00};
   for (int y = 0; y < 11; y++) {
      uint32_t delta = 0;
      for (int x = 0; splash[y][1][x] != 0; x++)
         if (splash[y][1][x] != 0xc2)
            render(font8x8_basic[(int)splash[y][1][x]], 100 + (x - delta) * 8, 100 + y * 12, color[y]);
         else 
            delta++;
   }
}

int kernel_start()
{
	splash_screen();
	splash_vga_screen();

	printf("Keyboard initialization...");
	kbd_init();
	printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

	printf("Scheduler initialization...");
	sched_init();
	printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

	printf("Hardware memory manager initialization...");
	extern char _free_memory_start[];
	extern char _memory_end[];
	hmm_init(_free_memory_start, _memory_end);
	printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");
	printf("\tNumber of frames %d = %d ko\n", hmm_frame_count(), hmm_free_memory() >> 10);

	printf("Clock interruptions...");
	clock_init();
	printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

	printf("Virtual memory manager initialization...");
	init_virtual_memory();
	if (is_virtual_memory_enable()) {
                printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

		printf("Shared memory API initialization...");
		shm_init();
                printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

		printf("Syscall initialization...");
		sysc_init();
                printf("\r\t\t\t\t\t\t\t[  \x1b[2mOK\x1b[0m  ]\n");

		sched_kstart(launcher, 10, "Launcher", 0);

	} else {
                printf("\r\t\t\t\t\t\t\t[  \x1b[9mNOT AVAILABLE\x1b[0m  ]\n");
		printf("# Virtual memory not available, start kernel tests\n");
		sched_kstart(kernel_tests, 10, "Launcher", 0);
	}

	assert(0 && "end of main");
}
