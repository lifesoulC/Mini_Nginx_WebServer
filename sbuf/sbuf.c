#include "sbuf.h"

void sbuf_init(sbuf_t *sp, int n, int itemSize)
{
	sp->buf = malloc(n*itemSize);
	sp->n = n;
	sp->front = sp->rear = 0;
	Sem_init(&sp->mutex, 0, 1);
	Sem_init(&sp->slots, 0, n);
	Sem_init(&sp->items, 0, 0);
}

void sbuf_free(sbuf_t *sp)
{
    free(sp->buf);
    sp->buf = NULL;	
}

void sbuf_insert(sbuf_t *sp, void *item, int itemSize)
{
	if(item == NULL)
	{
		printf("%s\n",pInputNull);
		return;
	}
	P(&sp->slots);
	P(&sp->mutex);
	int n = sp->rear%sp->n;
	memcpy(sp->buf+n*itemSize, item, itemSize);
	V(&sp->mutex);
	V(&sp->items);
}

void *sbuf_remove(sbuf_t *sp, int itemSize)
{
	void *item = NULL;
	int n = sp->front%sp->n;
	P(&sp->items);
	P(&sp->mutex);
	item = sp->buf+n*itemSize;
	V(&sp->mutex);
	V(&sp->slots);
	return item;
}
