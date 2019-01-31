/*
 * Damien Dejean - Gaëtan Morin
 * Ensimag, Projet Système 2010
 *
 * XUNIL
 * Appels systèmes
 */

#include "syscall.h"
#include "scheduler.h"
#include "process.h"
#include "stddef.h"
#include "cons_write.h"
#include "kbd.h"
#include "timer.h"

extern char userspace_end[];

/*
 * Vérification des pointeurs
 */

#define INVALID_PTR(ptr) \
        ( \
				( (char*)(ptr) < (char*) PROCESS_CODE && (char*)(ptr) != NULL) \
                || \
                (char*) (ptr) > (char*) KERNEL_CODE - sizeof(*ptr) \
                )



/*
 * Implémentation d'un appel système :
 *
 * - Écrire ici la fonction checked_<nom de l'appel systeme>() qui fait les
 *   vérifications de pointeurs.
 *   Si aucune vérification n'est nécessaire, ce nom de fonction peut être
 *   défini avec une simple macro.
 *
 * - Ajouter dans la fonction sysc_init en bas de la page un appel à
 *   BIND_SYSCALL(appel systeme)
 */

#define checked_beep            pcspkr_beep

#define checked_chprio          sched_chprio

static void checked_clock_settings(unsigned long *q, unsigned long *t)
{
	if (INVALID_PTR(q) || INVALID_PTR(t))
		return;
	clock_settings(q, t);
}

#define checked_cons_echo       cons_echo

/*
static int checked_cons_chbuffer(unsigned char *buf, font *f, int w, int h)
{
	if (UNLIKELY(INVALID_PTR(buf) || INVALID_PTR(f)))
		return RET_ERROR;
	return vesa_text_mode(buf, f, w, h);
}
*/

static unsigned long checked_cons_read(char *str, unsigned long l)
{
	if (INVALID_PTR(str))
		return 0;
	return cons_read(str, l);
}


static int checked_cons_write(char *str, long l)
{
	if (INVALID_PTR(str))
		return -1;
	return cons_write(str, l);
}

#define checked_cons_wait       cons_wait

#define checked_current_clock   current_clock

#define checked_exit            sched_exit

/*
static int checked_getname(int pid, char *buf, unsigned int size)
{
	if (UNLIKELY(INVALID_PTR(buf)))
		return RET_ERROR;
	return process_getname(pid, buf, size);
}
*/

#define checked_getpid          sched_get_active_pid

#define checked_getprio         process_getprio

/*
static int checked_getstatus(int pid, char *buf, unsigned int size)
{
	if (UNLIKELY(INVALID_PTR(buf)))
		return RET_ERROR;
	return process_getstatus(pid, buf, size);
}
*/

#define checked_kill            sched_kill

/*
static int checked_pcount(int fid, int *count)
{
	if (UNLIKELY(INVALID_PTR(count)))
		return RET_ERROR;
	return pcount(fid, count);
}
*/

#define checked_pcreate         pcreate

#define checked_pdelete         pdelete

/*
static int checked_preceive(int fid, int *msg)
{
	if (UNLIKELY(INVALID_PTR(msg)))
		return RET_ERROR;
	return preceive(fid, msg);
}
*/

#define checked_preset          preset

#define checked_psend           psend

/*
static int checked_psize(int fid, int *size)
{
	if (UNLIKELY(INVALID_PTR(size)))
		return RET_ERROR;
	return psize(fid, size);
}
*/

#define checked_sleep           sleep

#define checked_sleepms         sleepms

static int checked_start(const char *name,
						 unsigned long ssize,
						 int prio,
						 void *arg)
{
	if (INVALID_PTR(name))
		return -1;
	return sched_ustart(name, ssize, prio, arg);
}

#define checked_wait_clock      wait_clock

static int checked_waitpid(int pid, int64_t *rv)
{
	if (INVALID_PTR(rv))
		return -1;
	return sched_waitpid(pid, rv);
}

/*
static int checked_waitpid_nohang(int pid, int *rv)
{
	if (UNLIKELY(INVALID_PTR(rv)))
		return RET_ERROR;
	return sched_waitpid_nohang(pid, rv);
}
*/

#define checked_shm_create      shm_create
#define checked_shm_acquire     shm_acquire
#define checked_shm_release     shm_release


/*
 * Bindings des appels systèmes
 */

extern void syscall_wrapper(void);      /* Traitant d'interruption */

static void sysc_error_handler(void)
{
	assert(0 && "Appel systeme non implemente");
}

#define BIND_SYSCALL(name) (sysc_handlers[SYSC_##name] = checked_##name)


void sysc_init(void)
{
	int i;
	for (i = 0; i < NB_SYSCALLS; ++i)
		sysc_handlers[i] = sysc_error_handler;

	BIND_SYSCALL(chprio);
	BIND_SYSCALL(clock_settings);
	BIND_SYSCALL(cons_echo);
	BIND_SYSCALL(cons_read);
	BIND_SYSCALL(cons_write);
	BIND_SYSCALL(current_clock);
	BIND_SYSCALL(exit);
	BIND_SYSCALL(getpid);
	BIND_SYSCALL(getprio);
	BIND_SYSCALL(kill);
//	BIND_SYSCALL(pcount);
//	BIND_SYSCALL(pcreate);
//	BIND_SYSCALL(pdelete);
//	BIND_SYSCALL(preceive);
//	BIND_SYSCALL(preset);
//	BIND_SYSCALL(psend);
	BIND_SYSCALL(start);
	BIND_SYSCALL(wait_clock);
	BIND_SYSCALL(waitpid);
	BIND_SYSCALL(sleep);
//	BIND_SYSCALL(getname);
//	BIND_SYSCALL(getstatus);
//	BIND_SYSCALL(psize);
//	BIND_SYSCALL(waitpid_nohang);
	BIND_SYSCALL(sleepms);
//	BIND_SYSCALL(beep);
//	BIND_SYSCALL(cons_chbuffer);
//	BIND_SYSCALL(cons_wait);

//	BIND_SYSCALL(shm_create);
//	BIND_SYSCALL(shm_acquire);
//	BIND_SYSCALL(shm_release);

	/* Affectation du traitant d'interruption */
// todo regarder si on peut activer les interruptinos syscall indépendament.
}



