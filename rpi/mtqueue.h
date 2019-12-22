// mtqueue is a multi-threaded queue of pointers
#ifndef MTQUEUE_H
#define MTQUEUE_H

#include "zqueue.h"
#include <pthread.h>

typedef struct mtqueue mtqueue_t;
struct mtqueue
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    zqueue_t *      queue;
};

mtqueue_t *mtqueue_create();

void *mtqueue_pop_blocking(mtqueue_t *mtq);

/**
 * Note: poll() cannot eliminate race conditions that users might
 * create with concurrent push().
 **/
void *mtqueue_pop_poll(mtqueue_t *mtq);

/**
 * Note: size() is usually not safe. Its return value is only
 * meaningful if you can guarantee that there are no other concurrent
 * accesses.
 **/
int mtqueue_size(mtqueue_t *mtq);

void mtqueue_push(mtqueue_t *mtq, void *obj);

void mtqueue_destroy(mtqueue_t *mtq);


void mtqueue_lock(mtqueue_t *mtq);
int mtqueue_locked_size(mtqueue_t *mtq);
void *mtqueue_locked_get(mtqueue_t *mtq, int idx);
void mtqueue_locked_remove(mtqueue_t *mtq, int idx);
void mtqueue_unlock(mtqueue_t *q);

#endif
