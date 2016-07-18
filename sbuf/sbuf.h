#ifndef _SBUF_H
#define _SBUF_H

#include "../defs.h"
#include "../pub.h"

typedef struct tagTsbuf
{
	void *buf;
	int n;
	int front;
	int rear;
	sem_t mutex;
	sem_t slots;
	sem_t items;
}sbuf_t;

extern int Sem_init(sem_t *sem, int pshared, unsigned int value);
extern int P(sem_t *s);
extern int V(sem_t *s);

extern char *pInputNull;

void sbuf_init(sbuf_t *sp, int n, int itemSize);
void sbuf_free(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, void *item, int itemSize);
void *sbuf_remove(sbuf_t *sp, int itemSize);
#endif
