/* $COPYRIGHT_UM
$LICENSE_BSD
*/

#pragma once

#include "zarray.h"

typedef struct workerpool workerpool_t;

// as a special case, if nthreads==1, no additional threads are
// created, and workerpool_run will run synchronously.
workerpool_t *workerpool_create(int nthreads);
void workerpool_destroy(workerpool_t *wp);

void workerpool_add_task(workerpool_t *wp, void (*f)(void *p), void *p);

// runs all added tasks, waits for them to complete.
void workerpool_run(workerpool_t *wp);

// same as workerpool_run, except always single threaded. (mostly for debugging).
void workerpool_run_single(workerpool_t *wp);

int workerpool_get_nthreads(workerpool_t *wp);

int workerpool_get_nprocs();
