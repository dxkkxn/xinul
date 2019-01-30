/*
 * Damien Dejean - Gaëtan Morin
 * Ensimag, Projet Système 2010
 *
 * XUNIL
 * Traitement des entrées clavier.
 */

//#include "cpu.h"
//#include "interrupt.h"
#include "kbd.h"
#include "scheduler.h"
#include "buf.h"
#include "queue.h"
//#include "terminal.h"
#include "stddef.h"
#include "mem.h"
#include "string.h"
#include "stdio.h"
#include "cons_write.h"

#include "csr.h"
#include "encoding.h"

#define KBD_DATA_PORT           0x60
#define KBD_CMD_PORT            0x64

#define ENDL                    13
#define BACKSPACE               127


/* Buffer clavier */
static char buffer_data[KBD_BUFFER_SIZE];
static STRUCT_BUFFER(char) buffer;

/* Nombre de caractères 'fin de ligne' dans le buffer */
static int count_endl = 0;

/* File d'attente */
static link waitq;

/* Mode echo */
static int echo_on = 1;



/* initialise le clavier */
void kbd_init(void)
{
	INIT_BUFFER(&buffer, buffer_data, KBD_BUFFER_SIZE);
	INIT_LIST_HEAD(&waitq);

	// keyboard interruption already unmasked in PLIC in machine mode
	csr_set(sie, MIP_SEIP);
}

/* Traitant clavier */
void keyboard_handler()
{
	char s[2];
	int scancode = getchar();
	//do_scancode(scancode);
	if (scancode != -1 && echo_on) {
		//printf("%c", scancode);
		s[0] = (char) scancode;
		s[1] = '\0';
		keyboard_data(s);
	}
}



void cons_echo(int on)
{
	echo_on = on;
}


/* Envoie l'écho sur la sortie */
static inline void kbd_echo(char c)
{
	char buf[3];
	int  len = 0;
	static int line_len = 0;

	if (!echo_on)
		return;

	if ((c >= 32 && c <= 126) || c == 9) {
		++line_len;
		buf[len++] = c;
	} else if (c == ENDL) {
		line_len = 0;
		buf[len++] = '\n';
	} else if (c == BACKSPACE && line_len > 0) {
		--line_len;
		buf[len++] = '\b';
		buf[len++] = ' ';
		buf[len++] = '\b';
	} else if (c < 32) {
		++line_len;
		buf[len++] = '^';
		buf[len++] = 64 + c;
	}

	cons_write(buf, len);
}


/* Retourne une valeur non nulle si il faut scheduler */
static inline int kbd_accept(char c)
{
	char *last;
	process_t *p;
	int scheduling = 0;

	if (c == BACKSPACE) {
		kbd_echo(c);
		buf_unwrite(&buffer, last);
		if (last == NULL) {
			/* Le buffer est vide */
			// xbeep();
		} else if (*last == ENDL) {
			/* On remet le caractère */
			buf_write(&buffer, *last);
			// xbeep();
		}
		return 0;
	}
	/* Le caractère n'est pas un BACKSPACE */

	if (buf_full(&buffer)) {
		while (c == ENDL && (p = PROCESS_QUEUE_TOP(&waitq))) {
			scheduling = sched_unblock(p) || scheduling;
		}
		return scheduling;
	}
	/* Le buffer n'est pas plein */

	kbd_echo(c);
	buf_write(&buffer, c);
	if (c == ENDL) {
		++count_endl;
		if ((p = PROCESS_QUEUE_TOP(&waitq)))
			return sched_unblock(p);
	}
	return 0;
}


void keyboard_data(char *str)
{
	char c;
	int scheduling = 0;

	while ( (c = *(str++)) ) {
		scheduling = kbd_accept(c) || scheduling;
	}
	if (scheduling)
		schedule();
}

void kbd_leds(unsigned char leds)
{
	// Empty
}

unsigned long cons_read(char *string, unsigned long length)
{
	unsigned long i;
	char c;

	if (length == 0)
		return 0;

	while (count_endl == 0 && !buf_full(&buffer))
		sched_block(0, &waitq, BLOCKED_ON_IO, NULL);

	assert(!buf_empty(&buffer) && "Buffer clavier vide après déblocage");
	for (i = 0; i < length; ++i) {
		if (buf_empty(&buffer))
			break;

		buf_read(&buffer, c);
		if (c == ENDL) {
			--count_endl;
			assert(count_endl >= 0);
			break;
		}

		string[i] = c;
	}

	return i;
}


