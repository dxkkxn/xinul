/** See LICENSE for license details.
* process.c
*
*  Created on 15 novembre 2018
*/

#include "process.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include "virtual_memory.h"
#include "program.h"
#include "context.h"
#include "crt_process.h"


process_t* processes[NBPROC];

static inline int process_newpid(void)
{
	int i;
	for (i = 0; i < NBPROC && processes[i] != NULL; i++);
	if (i == NBPROC) {
		return -1;
	} else {
		return i + 1;
	}
}

static inline int pid_exists(int pid)
{
	return pid > 0 && pid <= NBPROC && processes[pid-1] != NULL;
}

#if 0
/**
 * Process initialization
 */
void init_process(void)
{
 // init idle process
	process_t* idle = malloc(sizeof( *idle));
	memset(idle, 0, sizeof(*idle));
	idle->name = malloc(strlen("idle")+1);
	strncpy(idle->name, "idle", MAX_LENGTH_PROCESS_NAME);
	idle->pid = 0;
	idle->prio = 0;
	idle->status = ACTIVE;
	table_process[0] = idle;
	process_alive = idle;
	nbproc = 1;
}
#endif



void process_exit() //TODO appeler l'ordonnanceur mettre le processus sur la file de destruction.
{
	printf("handler exit, je redonne la main...\n");
	process_t* idle = process_get(0);
	process_t* cur = process_get(1);
	ctx_sw(&cur->context, &idle->context);

}

void process_user_exit()
{
	process_exit();
}

#if 0
process_t* create_generic_process(const char *name, int priority)
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

	process_t* new = malloc(sizeof( *new));
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
	new->context.sp = (void*) &new->kernel_stack[STACK_SIZE-1];

	new->pid = pid;
	new->prio = priority;

	// Gestion du parent
	new->parent = process_alive;
	// Ajout du nouveau process à la liste des fils du parent
	//add_child(new);

	// Init de la liste des zones partagé par le process
	//new->list_shared_zone.next = &(new->list_shared_zone);
	//new->list_shared_zone.prev = &(new->list_shared_zone);

	// Attribution de l'état ready pour le process
	new->status = ACTIVABLE;

	nbproc++;
	// Ajout du process à la table
	table_process[pid] = new;
	pid++;
	if (pid == NBPROC) pid = 1;
	return new;
}
#endif

/* Allocate and initialize a process structure */
process_t* process_create(
		const char *name,
		unsigned long ssize,
		int prio,
		process_t *parent)
{
	process_t* p;
	int pid;
	uint8_t name_length;

	// Find the the pid
	pid = process_newpid();
	if (pid < 1) {
		return NULL;
	}

	// allocate the process structure
	p = malloc(sizeof(*p));
	if (p == NULL) {
		return NULL;
	}
	memset(p, 0, sizeof(*p));

	// copy the name of the process in the structure
	name_length = strlen(name);
	if (name_length > MAX_LENGTH_PROCESS_NAME) {
		name_length = MAX_LENGTH_PROCESS_NAME;
	}
	p->name = malloc(name_length + 1);
	if (p->name == NULL) {
		return NULL;
	}
	strncpy(p->name, name, name_length);
	p->name[name_length] = 0;

	// Kernel stack allocation
	p->kernel_stack = malloc(K_STACK_SIZE);
	if (p->kernel_stack == NULL) {
		return NULL;
	}

	// Structure informations
	p->pid = pid;
	p->prio = prio;
	p->parent = parent;
	INIT_LIST_HEAD(&p->children);
	p->info = 0;
	p->error = 0;

	// register the created process in the table and return its pointer
	processes[pid-1] = p;
	return p;
}

/* Destroy the "pid" process */
int process_destroy(int pid)
{
	process_t* p;
	if (pid < 1 || pid > NBPROC) {
		return -1;
	}
	p = processes[pid-1];

	free(p->name);
	free(p->kernel_stack);
	free(p);

	processes[pid-1] = NULL;

	return 0;
}

#if 0
/**
 * Créer un nouveau process à partir de son code et de son nom,
 *
 * retourne NULL si erreur.
 */
process_t* create_kernel_process(int (*code)(void *), const char *name, int priority, void *arg)
{
	if (priority < 1 || priority > MAXPRIO_KERNEL) {
		return NULL;
	}

	process_t* new = create_generic_process(name, priority);
	if (new == NULL) return NULL;
	new->context.satp = get_kernel_satp().reg;
	new->context.ra = (void*) crt_process;
	new->context.s0 = process_exit;
	new->context.s1 = arg;
	new->context.sepc = code;

	return new;
}


process_t* create_user_process(const char *code_name, const char *nom, int priority, int stack_size, void *arg)
{
	if (priority < 1 || priority > MAXPRIO) {
		return NULL;
	}

	if (stack_size < 0
			|| stack_size > STACK_SIZE_MAX) {
		return NULL;
	}
	// On place une taille de stack min à 100 TODO demander au prof
	if (stack_size < 100) {
		stack_size = 100;
	}

	// On recherche le code du programme
	void* code = hello_user;
	//const struct uapps *app = NULL;
	//app = symbol_map_get(code_name, NULL);
	//if (app == NULL) {
//		return NULL;
//	}

	process_t* new = create_generic_process(nom, priority);
	if (new == NULL) return NULL;

	// Mise en place de la mémoire virtuelle
	new->context.satp = init_user_virtual_memory(new->pid).reg;

	//HEAP
	//Tas User
	//if (alloc_region(pagedir, MEM_USER_HEAP, MEM_USER_HEAP + 4*HEAP_USER_SIZE, PAGE_TABLE_USER_RW) == -1){
		//return NULL;
	//}

	// STACK
	// Stack user
	new->user_stack_size = stack_size;
	// on alloue la pile
	void* user_stack = calloc(1, new->user_stack_size);
	if (user_stack == NULL) {
 	return NULL;
	}

	new->context.sp = user_stack;
	new->context.ra = (void*) crt_user_process;
	new->context.s0 = process_user_exit;
	new->context.s1 = arg;
	new->context.s2 = code;

	// Copie du programme pour le processus
	//alloc_and_copy_program(pagedir, app->start, app->end);
	return new;
}
#endif

/* Return the priority of the process pid if it exists, else -1 */
int process_getprio(int pid)
{
	if (!pid_exists(pid)) {
		return -1;
	}

	// We cannot get the priority of a zombie process
	if (processes[pid-1]->status == ZOMBIE) {
		return -1;
	}

	return processes[pid-1]->prio;
}

/* Return the processus matching with the given pid */
process_t* process_get(int pid)
{
	if (pid > 0 && pid <= NBPROC) {
		return processes[pid-1];
	} else {
		return NULL;
	}
}
