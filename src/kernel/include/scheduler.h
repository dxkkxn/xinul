#ifndef SCHEDULER_H
#define SCHEDULER_H


#include "queue.h"
#include "process.h"
#include "machine.h"


#define PROCESS_QUEUE_TOP(head)		queue_top(head, process_t, status_link)
#define PROCESS_QUEUE_BOTTOM(head)	queue_bottom(head, process_t, status_link)

/* Possible states of the processes */
enum p_status {
	/* Executing */
	ACTIVE = -1,

	/* States handled by the scheduler */
	ACTIVABLE = 0,
	ZOMBIE,
	BLOCKED_ON_WSON,
	NB_SCHED_STATES,

	/* Other states */
	BLOCKED_ON_MSG = NB_SCHED_STATES,
	BLOCKED_ON_IO,
	BLOCKED_ON_CLOCK,
	NB_STATES
};

/* Scheduler initialization */
void sched_init(void);

/* Start of a process: */
/*  - kernel */
int sched_kstart(int (*run) (void *),
                 int prio,
                 const char *name,
                 void *arg);

/*  - user */
int sched_ustart(const char *name,
                 unsigned long ssize,
                 int prio,
                 void *arg);

/* Scheduling: select the next active process */
void schedule(void);

/* End the current process with its returned value, reboot if pid = 1 */
void sched_exit(int retval);

/* Kill the process of the given pid */
int sched_kill(int pid);

/* Return the pid of the active process */
int sched_get_active_pid(void);

/*
 * Bock a process in the given queue
 * info         : information to store in the process
 * queue        : priority queue
 * new_status   : ne status of the process
 * error        : if not null, take of value of the process error field one
 *                the process is unblocked
 * Return the info field value
 */
int sched_block(long info, link *queue, int new_status, int *error);

/*
 * Unblock a previously blocked process (with sched_block)
 * Return != 0 if the scheduler must be called
 */
int sched_unblock(process_t *p);

/* Wait one of the child of the active process
 * pid :        pid of the son of the child (or -1 for the first to end)
 * retvalp:     pointer on the return value of the son
 */
int sched_waitpid(int pid, int *retvalp);

/* Non blocking sched_waitpid()
 * pid :        pid of the son of the child (or -1 for the first to end)
 * retvalp:     pointer on the return value of the son
 */
int sched_waitpid_nohang(int pid, int *retvalp);

/*
 * Change the priority of a process and signal if to the scheduler if it is
 * in the activable list
 */
int sched_chprio(int pid, int newprio);

/*
 * Write buffer with the state of the process
 */
int sched_printstatus(process_t *p, char *buffer, unsigned int size);

#endif
