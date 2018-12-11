#include "scheduler.h"

#include "stdio.h"
#include "csr.h"

static link processes[NB_SCHED_STATES]; /* Files de processus du scheduler */
static link dustbin;
static struct context dummy;

#define STATUS_QUEUE_ADD(ptr, new_status)                               \
        do {                                                            \
                ptr->current_queue = &processes[new_status];                \
                ptr->state = new_status;                               \
                queue_add(ptr, &processes[new_status],                  \
                                struct desc_proc, status_link, prio);          \
        } while (0)

#define STATUS_QUEUE_DELETE(ptr)                                        \
        do {                                                            \
                ptr->current_queue = NULL;                              \
                queue_del(ptr, status_link);                            \
        } while (0)


void sched_init(){
  init_process(); //initialiser les processus et IDLE
  process_t kernel_idle = get_process(0); //IDLE processus
  /* Initialiser les têtes de listes */
  int i;
  for (i = 0; i < NB_SCHED_STATES; ++i)
          INIT_LIST_HEAD(&processes[i]);
  INIT_LIST_HEAD(&kernel_idle->children);
  INIT_LIST_HEAD(&dustbin);

  //TODO: restaurer le kernelinit
  //context_kernelinit(&kernel_idle->context, kernel_idle->kernel_stack,
  //                   idle, NULL);

  /* Le premier processus à s'exécuter */
  STATUS_QUEUE_ADD((kernel_idle), ACTIVABLE);
}

/*Tue un processus ou le fait devenir zombie s'il a un pere*/
static void sched_zombify(process_t p)
{
        //process_t parent = p->parent;

        //sched_cleanup_children(p); //TODO implanter cette fonction

        //if (parent == NULL) {
                /* Marquage pour une destruction différée */
                queue_add(p, &dustbin, struct desc_proc, status_link, prio);
                return;
        //}

        /* Passe en zombie et débloque son père */
        /*STATUS_QUEUE_ADD(p, ZOMBIE);
        if (parent->status == BLOCKED_ON_WSON &&
                        (parent->info < 0 || p->pid == parent->info)) {
                sched_unblock(parent);
        }*/
}
/* Fonction qui nettoye les processus finis*/
void exit_scheduling()
{
	printf("exit scheduler -> donne la main\n");
	process_t p = process_alive;
	process_alive = NULL;
	//p->info = retval;
	p->error = 0;
	sched_zombify(p);
	schedule();
	assert(0 && "Retour de sched_exit()");
}

int sched_kstart(int (*run) (void *),
                 int prio,
                 const char *name,
                 void *arg)
{
        /* On initialise le processus */
        process_t p = create_kernel_process(run, name , prio, arg);
        if (p == NULL && run == NULL)
                return -1;

        /* On initialise son contexte */
        STATUS_QUEUE_ADD(p, ACTIVABLE);
      //  if (active != NULL)
      //          FAMILY_QUEUE_ADD(p, active);
      p->context.s0 = exit_scheduling;
      return p->pid;
}
void schedule(){
  printf("scheduling...\n");
  process_t old = process_alive;
  process_t new = queue_top(&processes[ACTIVABLE], struct desc_proc, status_link);
  struct context *old_ctx;
  if (new == NULL) {
                assert(old != NULL && "Il n'y a plus de processus !");
                return;
  }
  if (old == NULL || old->state != ACTIVE || old->prio <= new->prio) {
                /* Extraire le prochain processus disponible */
                STATUS_QUEUE_DELETE(new);
                process_alive = new;

                if (old != NULL && old->state == ACTIVE)
                        STATUS_QUEUE_ADD(old, ACTIVABLE);
  }
  process_alive->state = ACTIVE;
  old_ctx = (old != NULL)? &old->context : &dummy;
  ctx_sw(old_ctx, &new->context);

  /* Détruit les processus marqués à supprimer */
  //while ( (old = queue_out(&dustbin, struct desc_proc, status_link)) )
  //              process_destroy(old->pid);//TODO effacer le processus de l'array.
}
