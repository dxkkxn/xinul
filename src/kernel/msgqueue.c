/*
 * Damien Dejean - Gaëtan Morin
 * Ensimag, Projet Système 2010
 *
 * XUNIL
 * Files de messages.
 */

#include "msgqueue.h"
#include "mem.h"
#include "stddef.h"


int msg_init(msgqueue_t *mq, int size)
{
        int *data;

        if (size >= (int)(0xFFFFFFFF / sizeof(*data)))
                return -1;

        if (size < 1)
                return -1;

        data = mem_alloc(size * sizeof(*data));
        if (data == NULL)
                return -1;

        INIT_BUFFER(&mq->buf, data, size);
        INIT_LIST_HEAD(&mq->waitq);

        return 0;
}



int msg_send(msgqueue_t *mq, int msg)
{
        int error = 0;
        process_t *p = PROCESS_QUEUE_TOP(&mq->waitq);

        if (buf_empty(&mq->buf) && p != NULL) {
                /* On affecte la valeur de retour de sched_block() */
                p->info = msg;
                if (sched_unblock(p))
                        schedule();
        } else if (buf_full(&mq->buf)) {
                sched_block(msg, &mq->waitq, BLOCKED_ON_MSG, &error);
        } else {
                buf_write(&mq->buf, msg);
        }

        return error;
}



int msg_receive(msgqueue_t *mq, int *msg)
{
        int readmsg;
        int error = 0;
        process_t *p = PROCESS_QUEUE_TOP(&mq->waitq);
        
        if (buf_empty(&mq->buf)) {
                readmsg = sched_block(0, &mq->waitq, BLOCKED_ON_MSG, &error);
        } else if (buf_full(&mq->buf) && p != NULL) {
                buf_rotate(&mq->buf, readmsg, p->info);
                if (sched_unblock(p))
                        schedule();
        } else {
                buf_read(&mq->buf, readmsg);
        }

        if (msg != NULL && error != -1)
                *msg = readmsg;

        return error;
}



int msg_count(msgqueue_t *mq)
{
        process_t *p;
        int process_count = 0;
        
        queue_for_each(p, &mq->waitq, process_t, status_link) {
                ++process_count;
        }
        
        if (buf_empty(&mq->buf)) {
                return -process_count;
        }
        
        return mq->buf.count + process_count;
}



int msg_size(msgqueue_t *mq)
{
        return mq->buf.size;
}



void msg_reset(msgqueue_t *mq)
{
        int scheduling = 0;
        process_t *p = PROCESS_QUEUE_TOP(&mq->waitq);
        
        if (buf_empty(&mq->buf) && p == NULL)
                return;

        buf_reset(&mq->buf);

        while (p != NULL) {
                p->error = -1;
                scheduling = sched_unblock(p) || scheduling;
                p = PROCESS_QUEUE_TOP(&mq->waitq);
        }

        if (scheduling)
                schedule();
}


/*
 * API utilisateur
 */
struct msgq_slot {
        int allocated;
        msgqueue_t mq;
};

static struct msgq_slot queues[NBQUEUE];


static inline int mq_newfid(void)
{
	int i;
	for (i = 0; i < NBQUEUE && queues[i].allocated; ++i);
	if (i == NBQUEUE)
		return -1;
	
        queues[i].allocated = 1;
	return i+1;

}


static inline msgqueue_t *mq_get(int fid)
{
        int i = fid-1;
        if (fid < 1 || fid > NBQUEUE || !queues[i].allocated)
                return NULL;
        
        return &(queues[i].mq);
}


static inline void mq_free(int fid)
{
        queues[fid-1].allocated = 0;
}


int pcreate(int count)
{
        int fid;
        if (count <= 0)
                return -1;

        fid = mq_newfid();
        if (fid < 1)
                return -1;

        if (msg_init(mq_get(fid), count) == -1)
                return -1;

        return fid;
}

int pdelete(int fid)
{
	msgqueue_t *mq = mq_get(fid);
	if (mq == NULL)
		return -1;

	buf_free_data(&mq->buf);
	mq_free(fid);
	msg_reset(mq);
	return 0;
}

int preceive(int fid, int *message)
{
        msgqueue_t *mq = mq_get(fid);
        if (mq == NULL)
                return -1;

        return msg_receive(mq, message);
}

int psend(int fid, int message)
{
        msgqueue_t *mq = mq_get(fid);
        if (mq == NULL)
                return -1;

        return msg_send(mq, message);
}

int preset(int fid)
{
        msgqueue_t *mq = mq_get(fid);
        if (mq == NULL)
                return -1;

        msg_reset(mq);
        return 0;
}

int pcount(int fid, int *count)
{
        msgqueue_t *mq = mq_get(fid);
        if (mq == NULL)
                return -1;

        *count = msg_count(mq);
        return 0;
}

int psize(int fid, int *size)
{
        msgqueue_t *mq = mq_get(fid);
        if (mq == NULL)
                return -1;

        *size = msg_size(mq);
        return 0;

}


