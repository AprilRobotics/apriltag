/* $COPYRIGHT_UM
$LICENSE_BSD
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct timeutil_rest timeutil_rest_t;
timeutil_rest_t *timeutil_rest_create();
void timeutil_rest_destroy(timeutil_rest_t * rest);

int64_t utime_now(); // blacklist-ignore
int64_t utime_get_seconds(int64_t v);
int64_t utime_get_useconds(int64_t v);
void    utime_to_timeval(int64_t v, struct timeval *tv);
void    utime_to_timespec(int64_t v, struct timespec *ts);

int32_t  timeutil_usleep(int64_t useconds);
uint32_t timeutil_sleep(unsigned int seconds);
int32_t  timeutil_sleep_hz(timeutil_rest_t *rest, double hz);

void timeutil_timer_reset(timeutil_rest_t *rest);
void timeutil_timer_start(timeutil_rest_t *rest);
void timeutil_timer_stop(timeutil_rest_t *rest);
bool timeutil_timer_timeout(timeutil_rest_t *rest, double timeout_s);

int64_t time_util_hhmmss_ss_to_utime(double time);

int64_t timeutil_ms_to_us(int32_t ms);

#ifdef __cplusplus
}
#endif
