/*
 * Damien Dejean - Gaëtan Morin
 * Ensimag, Projet Système 2010
 *
 * XUNIL
 * Files de messages.
 */

#ifndef __MSG_QUEUE_H___
#define __MSG_QUEUE_H___

#include "scheduler.h"
#include "queue.h"
#include "buf.h"

#define NBQUEUE 256

/*
 * Manipulation directe de files de messages
 */
typedef struct {
        STRUCT_BUFFER(int) buf; /* Buffer de messages */
        link waitq;             /* File d'attente de processus */
} msgqueue_t;

int msg_init(msgqueue_t *mq, int size);
int msg_send(msgqueue_t *mq, int msg);
int msg_receive(msgqueue_t *mq, int *msg);
int msg_count(msgqueue_t *mq);
int msg_size(msgqueue_t *mq);
void msg_reset(msgqueue_t *mq);

/*
 * API utilisateur
 */
int pcreate(int count);
int pdelete(int fid);
int preceive(int fid, int *message);
int psend(int fid, int message);
int preset(int fid);
int pcount(int fid, int *count);
int psize(int fid, int *size);

#endif

