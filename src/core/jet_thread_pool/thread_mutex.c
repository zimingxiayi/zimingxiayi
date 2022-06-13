#include "thread.h"

int 
thread_mutex_create(pthread_mutex_t *mutex)
{
	int err = 0;
	pthread_mutexattr_t attr;

	err = pthread_mutexattr_init(&attr);
	if(err != THREAD_POOL_OK)
	{
		fprintf(stderr, "pthread_mutexattr_init() failed, reason: %s\n", strerror(errno));
		return THREAD_POOL_ERROR;
	}

	err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
	if(err != THREAD_POOL_OK)
	{
		fprintf(stderr, "pthread_mutexattr_settype(PTHREAD_MUTEX_ERRORCHECK) failed, reason: %s\n", strerror(errno));
		return THREAD_POOL_ERROR;	
	}

	err = pthread_mutex_init(mutex, &attr);
	if(err != THREAD_POOL_OK)
	{
		fprintf(stderr, "pthread_mutex_int() failed, reason: %s\n", strerror(errno));
		return THREAD_POOL_ERROR;
	}

	err = pthread_mutexattr_destroy(&attr);
	if(err != THREAD_POOL_OK)
	{
		fprintf(stderr, "pthread_mutexattr_destroy() failed, reason: %s\n", strerror(errno));
		return THREAD_POOL_ERROR;
	}

	return THREAD_POOL_OK;
}

int
thread_mutex_destroy(pthread_mutex_t *mutex)
{
	int err;

	err = pthread_mutex_destroy(mutex);
	if(err != THREAD_POOL_OK)
	{
		fprintf(stderr, "pthread_mutex_destroy() failed, reason: %s\n", strerror(errno));
		return THREAD_POOL_ERROR;
	}
	return THREAD_POOL_OK;
}

int	
thread_mutex_lock(pthread_mutex_t *mutex)
{
	int err;

	err = pthread_mutex_lock(mutex);
	if(err == THREAD_POOL_OK)
	{
		return THREAD_POOL_OK;
	}
	fprintf(stderr, "pthread_mutex_lock() failed, reason: %s\n", strerror(errno));

	return THREAD_POOL_ERROR;
}

int
thread_mutex_unlock(pthread_mutex_t *mutex)
{
	int err;

	err = pthread_mutex_unlock(mutex);

	if(err == 0)
	{
		return THREAD_POOL_OK;
	}
	fprintf(stderr, "pthread_mutex_unlock() failed, reason: %s\n", strerror(errno));

	return  THREAD_POOL_ERROR;
}
