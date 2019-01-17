/*
 * Damien Dejean - Gaëtan Morin
 * Mathieu Barbe
 * Ensimag, Projet Système 2010
 *
 * XUNIL
 * Headers de la bibliothèque d'appels systèmes.
 */
#ifndef ___SYSCALL_H___
#define ___SYSCALL_H___

int chprio(int pid, int newprio);

void clock_settings(unsigned long *quartz, unsigned long *ticks);

void cons_echo(int on);

unsigned long cons_read(char *string, unsigned long length);

int cons_write(const char *str, long size);

int cons_wait(void);

unsigned long current_clock(void);

void exit(int retval);

int getname(int pid, char *buffer, unsigned int size);

int getpid(void);

int getprio(int pid);

int getstatus(int pid, char *buffer, unsigned int size);

int kill(int pid);

int pcount(int fid, int *count);

int pcreate(int count);

int pdelete(int fid);

int preceive(int fid, int *message);

int preset(int fid);

int psend(int fid, int message);

int psize(int fid, int *size);

int start(int (*ptfunc)(void *), unsigned long ssize, int prio, const char * name, void *arg);

void wait_clock(unsigned long clock);

int waitpid(int pid, int *retvalp);

int waitpid_nohang(int pid, int *retvalp);

void sleep(unsigned long sec);

void sleepms(unsigned long ms);


void beep(unsigned long freq, unsigned long time);

//void cons_chbuffer(unsigned char *buffer, font *f, int width, int height);

/* Added for shared memory */
void *shm_created(const char*);
void *shm_acquire(const char*);
void shm_release(const char*);

#endif

