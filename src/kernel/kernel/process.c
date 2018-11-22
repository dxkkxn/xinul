/** See LICENSE for license details.
* process.c
*
*  Created on 15 novembre 2018
*/

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include "process.h"

// Process table
static process_t table_process[NBPROC];

// number of active process
static size_t nbproc;
// Current process
static process_t process_alive = NULL;

/**
 * Process initialization
 */
void init_process(void)
{
 // init idle process
	process_t idle = malloc(sizeof( *idle));
	memset(idle, 0, sizeof(*idle));
	idle->name = malloc(strlen("idle")+1);
	strncpy(idle->name, "idle", MAX_LENGTH_process_NAME);
	idle->pid = 0;
	idle->priority = MAXPRIO_KERNEL-1;
	idle->state = RUN;
	table_process[0] = idle;
	process_alive = idle;
	nbproc = 1;
}


void process_exit()
{
 printf("handler exit todo\n");
}

process_t create_generic_process(const char *name, int priority)
{
	// pid counter  (start at 1, idlepid is 0)
	static int pid = 1;

 // Is full? 
	if (nbproc == NBPROC) return NULL;

	// Search free pid
	while (table_process[pid] != 0x0){
	    pid++;
		if (pid == NBPROC) pid = 1;
	}
 
	process_t new = malloc(sizeof( *new));
	if (new == NULL) return NULL;
	memset(new, 0, sizeof( *new));
	uint8_t name_length = strlen(name);
	if (name_length > MAX_LENGTH_process_NAME) {
	 name_length = MAX_LENGTH_process_NAME;
	}
	new->name = malloc(name_length + 1);
	if (new->name == NULL) return NULL;
	strncpy(new->name, name, name_length);
	new->name[name_length] = 0;

	// Kernel stack init
	new->kernel_stack = malloc(STACK_SIZE);
	if (new->kernel_stack == NULL) return NULL;
	new->cpu_state.sp = (void*) &new->kernel_stack[STACK_SIZE-1];
 
	new->pid = pid;
	new->priority = priority;
 
	// Gestion du parent
	new->parent = process_alive;
	// Ajout du nouveau process à la liste des fils du parent
	//add_child(new);

	// Init de la liste des zones partagé par le process
	//new->list_shared_zone.next = &(new->list_shared_zone);
	//new->list_shared_zone.prev = &(new->list_shared_zone);

	// Attribution de l'état ready pour le process
	new->state = READY;
 
	nbproc++;
	// Ajout du process à la table
	table_process[pid] = new;
	pid++;
	if (pid == NBPROC) pid = 1;
	return new;
}

/**
 * Créer un nouveau process à partir de son code et de son nom,
 *
 * retourne NULL si erreur.
 */
process_t create_kernel_process(int (*code)(void *), const char *name, int priority, void *arg)
{
	if (priority < 1 || priority > MAXPRIO_KERNEL) {
		return NULL;
	}
 
 process_t new = create_generic_process(name, priority);
	if (new == NULL) return NULL;
	new->cpu_state.ra = (void*) code;
	new->cpu_state.a0 = arg;
	return new;
}

int getpid()
{
 return process_alive->pid;
}

process_t get_process(int pid)
{
 if (pid<0 || pid>=NBPROC )
  return NULL;
 return table_process[pid];
}
