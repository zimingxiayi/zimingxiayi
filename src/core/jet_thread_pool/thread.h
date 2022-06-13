#ifndef _THREAD_H_INCLUDE_
#define _THREAD_H_INCLUDE_

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

typedef intptr_t    int_t;
typedef uintptr_t   uint_t;

#define  THREAD_POOL_OK      0
#define  THREAD_POOL_ERROR  -1

int thread_mutex_create(pthread_mutex_t *mutex);
int thread_mutex_destroy(pthread_mutex_t *mutex);
int thread_mutex_lock(pthread_mutex_t *mutex);
int thread_mutex_unlock(pthread_mutex_t *mutex);

int thread_cond_create(pthread_cond_t *cond);
int thread_cond_destroy(pthread_cond_t *cond);
int thread_cond_signal(pthread_cond_t *cond);
int thread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

#endif
