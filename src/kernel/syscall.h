/*
 * Damien Dejean - Gaëtan Morin
 * Ensimag, Projet Système 2010
 *
 * XUNIL
 * Appels systèmes
 */

#include "syscall_num.h"

/* Tableau des traitants */
void *sysc_handlers[NB_SYSCALLS];

void sysc_init(void);


