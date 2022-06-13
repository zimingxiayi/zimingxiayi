#include "thread.h"

int
thread_cond_create(pthread_cond_t *cond)
{
	int err;
	err = pthread_cond_init(cond, NULL);
	if(err == 0) return THREAD_POOL_OK;

	fprintf(stderr, "pthread_cond_init() failed, reason: %s\n", strerror(errno));
	return THREAD_POOL_ERROR;
}

int
thread_cond_destroy(pthread_cond_t *cond)
{
	int err;
	err = pthread_cond_destroy(cond);
	if(err == 0) return THREAD_POOL_OK;

	fprintf(stderr, "pthread_cond_destroy() failed, reason: %s\n", strerror(errno));
	return THREAD_POOL_ERROR;
}

int
thread_cond_signal(pthread_cond_t *cond)
{
	int err;
	err = pthread_cond_signal(cond);
	if(err == 0) return THREAD_POOL_OK;

	fprintf(stderr, "pthread_cond_signal() failed, reason: %s\n", strerror(errno));
	return THREAD_POOL_ERROR;
}

int
thread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	int err;
	err = pthread_cond_wait(cond, mutex);
	if(err == 0) return THREAD_POOL_OK;

	fprintf(stderr, "pthread_cond_wait() failed, reason: %s\n", strerror(errno));
	return THREAD_POOL_ERROR;
}
