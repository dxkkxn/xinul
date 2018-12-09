#ifndef SCHEDULER_H
#define SCHEDULER_H


#include "process.h"
#include "machine.h"
/* Initialisation de l'ordonnanceur */
void sched_init(void);

/* Lancement d'un processus : */
/*  - kernel : Le processus pourra s'exécuter à partir du prochain schedule() */
int sched_kstart(int (*run) (void *),
                 int prio,
                 const char *name,
                 void *arg);

/*  - user : Le scheduler est appelé si besoin */
int sched_ustart(const char *name,
                 unsigned long ssize,
                 int prio,
                 void *arg);

/* Ordonnancement */
void schedule(void);

/* Terminaison d'un processus */
void sched_exit(int retval);

/* Tue le processus de pid donné */
int sched_kill(int pid);

/* PID du processus actif */
int sched_get_active_pid(void);

/*
 * Bloque un processus dans la file donnée en paramètre
 * info         : un champ d'information à stocker dans le processus
 * queue        : une file de priorité
 * new_status   : un status à donner au processus
 * error        : si non null, prend la valeur du champ error une fois
 *                le processus débloqué
 * Retourne la valeur du champ info une fois le processus débloqué
 */
int sched_block(long info, link *queue, int new_status, int *error);

/*
 * Débloque un processus précédemment bloqué avec sched_block()
 * Renvoie une valeur différente de 0 le scheduler doit
 * être appelé
 */
int sched_unblock(process_t *p);

/* Réordonne la file de priorité des processus actifs
 * après changement de la priorité du processus p
 * passé en paramètre */
void sched_reorder(process_t *p);

/* Attend l'un des fils du processus appelant
 * pid :        le pid du fils a attendre ou -1 pour le premier venu
 * retvalp:     pointeur sur la valeur de retour de fils (variable out)
 */
int sched_waitpid(int pid, int *retvalp);

/* Variante non bloquante de sched_waitpid()
 * pid :        le pid du fils a attendre ou -1 pour le premier venu
 * retvalp:     pointeur sur la valeur de retour de fils (variable out)
 */
int sched_waitpid_nohang(int pid, int *retvalp);

/*
 * Change la priorité d'un processus, et le signale à l'ordonnanceur si
 * il est dans la liste des activables.
 */
int sched_chprio(int pid, int newprio);

/*
 * Imprime dans un buffer l'état du processus p
 */
int sched_printstatus(process_t *p, char *buffer, unsigned int size);

#endif
