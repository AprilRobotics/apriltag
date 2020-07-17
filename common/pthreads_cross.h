#ifndef __CPTHREAD_H__
#define __CPTHREAD_H__

#ifdef _WIN32
#include <stdbool.h>
#include <windows.h>
#else
#include <pthread.h>
#include <sched.h>
#endif

#ifdef _WIN32

typedef CRITICAL_SECTION pthread_mutex_t;
typedef void pthread_mutexattr_t;
typedef void pthread_attr_t;
typedef void pthread_condattr_t;
typedef void pthread_rwlockattr_t;
typedef HANDLE pthread_t;
typedef CONDITION_VARIABLE pthread_cond_t;

#ifdef __cplusplus
extern "C" {
#endif
int pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
int pthread_join(pthread_t thread, void **value_ptr);
int pthread_detach(pthread_t);

int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);

int sched_yield(void);
#ifdef __cplusplus
}
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif
unsigned int pcthread_get_num_procs();

void ms_to_timespec(struct timespec *ts, unsigned int ms);
unsigned int timespec_to_ms(const struct timespec *abstime);
#ifdef __cplusplus
}
#endif

#endif /* __CPTHREAD_H__ */
