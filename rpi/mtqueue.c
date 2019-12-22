#include "mtqueue.h"
#include <assert.h>

mtqueue_t *mtqueue_create()
{
    mtqueue_t *mtq = calloc(1, sizeof(mtqueue_t));
    pthread_mutex_init(&mtq->mutex, NULL);
    pthread_cond_init(&mtq->cond, NULL);
    mtq->queue = zqueue_create(sizeof(void *));
    return mtq;
}

void *mtqueue_pop_blocking(mtqueue_t *mtq)
{
    pthread_mutex_lock(&mtq->mutex);
    while (zqueue_size(mtq->queue) == 0) {
        pthread_cond_wait(&mtq->cond, &mtq->mutex);
    }
    assert(zqueue_size(mtq->queue) >= 1);
    void *obj;
    zqueue_pop(mtq->queue, &obj);
    pthread_mutex_unlock(&mtq->mutex);

    return obj;
}

void *mtqueue_pop_poll(mtqueue_t *mtq)
{
    void *obj = NULL;

    pthread_mutex_lock(&mtq->mutex);
    if (zqueue_size(mtq->queue) > 0) {
        zqueue_pop(mtq->queue, &obj);
    }
    pthread_mutex_unlock(&mtq->mutex);

    return obj;
}

void mtqueue_push(mtqueue_t *mtq, void *obj)
{
    pthread_mutex_lock(&mtq->mutex);
    zqueue_push(mtq->queue, &obj);
    pthread_cond_broadcast(&mtq->cond);
    pthread_mutex_unlock(&mtq->mutex);
}

void mtqueue_destroy(mtqueue_t *mtq)
{
    if (!mtq) {
        return;
    }

    pthread_mutex_destroy(&mtq->mutex);
    pthread_cond_destroy(&mtq->cond);
    zqueue_destroy(mtq->queue);
    free(mtq);
}

int mtqueue_size(mtqueue_t *mtq)
{
    pthread_mutex_lock(&mtq->mutex);
    int sz = zqueue_size(mtq->queue);
    pthread_mutex_unlock(&mtq->mutex);

    return sz;
}

void mtqueue_lock(mtqueue_t *mtq)
{
    pthread_mutex_lock(&mtq->mutex);
}

void *mtqueue_locked_get(mtqueue_t *mtq, int idx)
{
    void *obj;
    zqueue_get(mtq->queue, idx, &obj);
    return obj;
}

void mtqueue_locked_remove(mtqueue_t *mtq, int idx)
{
    zqueue_remove_index(mtq->queue, idx);
}

int mtqueue_locked_size(mtqueue_t *mtq)
{
    return zqueue_size(mtq->queue);
}

void mtqueue_unlock(mtqueue_t *q)
{
    pthread_mutex_unlock(&q->mutex);
}
