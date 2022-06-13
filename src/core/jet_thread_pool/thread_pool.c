#include "thread_pool.h"

static void thread_pool_exit_handler(void *data);
static void *thread_pool_cycle(void *data);
static int_t thread_pool_init_default(thread_pool_t *tpp, char *name);

static uint_t  thread_pool_task_id;

static const int debug = 1;

thread_pool_t *thread_pool_create()
{
	int err = 0;
	pthread_t tid;
	uint_t  n;
	pthread_attr_t attr;
	thread_pool_t *tp = NULL;

	if(debug) fprintf(stderr, "%s start %s-%d\n", __func__, __FILE__, __LINE__);
	tp = (thread_pool_t *)calloc(1, sizeof(thread_pool_t));
	
	if(tp == NULL)
	{
		fprintf(stderr, "%s: calloc failed!\n", __func__);
		return NULL;
	}

	thread_pool_init_default(tp, NULL);

	thread_pool_queue_init(&tp->queue);

	if(thread_mutex_create(&tp->mutex)  != THREAD_POOL_OK)
	{
		free(tp);
		return NULL;
	}

	if(thread_cond_create(&tp->cond) != THREAD_POOL_OK)
	{
		thread_mutex_destroy(&tp->mutex);
		free(tp);
		return NULL;
	}

	err = pthread_attr_init(&attr);
	if(err)
	{
		fprintf(stderr, "pthread_attr_init() failed, reason: %s\n", strerror(errno));
		free(tp);
		return NULL;
	}

	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(err)
	{
		fprintf(stderr, "pthread_attr_setdetachstate() failed, reason: %s\n", strerror(errno));
		free(tp);
		return NULL;
	}

	for(n = 0; n < tp->threads; n++)
	{
		err = pthread_create(&tid, &attr, thread_pool_cycle, tp);
		if(err != THREAD_POOL_OK)
		{
			fprintf(stderr, "pthread_create() failed, reason: %s\n", strerror(errno));
			free(tp);
			return NULL;
		}

	}

	(void)pthread_attr_destroy(&attr);
	if(debug) fprintf(stderr, "%s end %s-%d\n", __func__, __FILE__, __LINE__);

	return tp;
}

static int_t 
thread_pool_init_default(thread_pool_t *tpp, char *name)
{
	if(tpp)
	{
		tpp->threads = DEFAULT_THREADS_NUM;
		tpp->max_queue = DEFAULT_QUEUE_NUM;

		tpp->name = strdup(name ? name : "default");
		return THREAD_POOL_OK;
	}
	return THREAD_POOL_ERROR;

}

void
thread_task_free(thread_task_t *task)
{
	if(task)
	{
		free(task);
	}
}

int_t 
thread_task_post(thread_pool_t *tp, thread_task_t *task)
{
	if(thread_mutex_lock(&tp->mutex) != THREAD_POOL_OK)
	{
		return THREAD_POOL_ERROR;
	}

	if(tp->waiting > tp->max_queue)
	{
		(void)thread_mutex_unlock(&tp->mutex);

		fprintf(stderr, "thread pool \"%s\" queue overflow: %ld tasks waiting\n",
				tp->name, tp->waiting);
		return THREAD_POOL_ERROR;
	}

	task->id = thread_pool_task_id++;
	task->next = NULL;

	if(thread_cond_signal(&tp->cond) != THREAD_POOL_OK)
	{
		(void)thread_mutex_unlock(&tp->mutex);
		return THREAD_POOL_ERROR;
	}

	*tp->queue.last = task;
	tp->queue.last = &task->next;

	tp->waiting++;

	(void)thread_mutex_unlock(&tp->mutex);

	if(debug) fprintf(stderr, "task #%lu added to thread pool \"%s\"\n", task->id, tp->name);

	return THREAD_POOL_OK;
}

thread_task_t *
thread_task_alloc(size_t size)
{
	thread_task_t *task;

	task = (thread_task_t *)calloc(1, sizeof(thread_task_t) + size);
	if(task == NULL)
	{
		return NULL;
	}

	task->ctx = task + 1;

	return task;
}

static void *
thread_pool_cycle(void *data)
{
	thread_pool_t *tp = (thread_pool_t*)data;
	thread_task_t *task;

	for(;;)
	{
		if(thread_mutex_lock(&tp->mutex) != THREAD_POOL_OK)
		{
			return NULL;
		}
		tp->waiting--;

		while(tp->queue.first == NULL)
		{
			if(thread_cond_wait(&tp->cond, &tp->mutex) != THREAD_POOL_OK)
			{
				(void)thread_mutex_unlock(&tp->mutex);
				return NULL;
			}
		}

		task = tp->queue.first;
		tp->queue.first = task->next;

		if(tp->queue.first == NULL)
		{
			tp->queue.last = &tp->queue.first;
		}

		if(thread_mutex_unlock(&tp->mutex) != THREAD_POOL_OK)
		{
			return NULL;
		}

		task->handler(task->ctx);

		task->next = NULL;

		if(debug) fprintf(stderr, "%s-%d\n free(task[%p])\n",__FILE__, __LINE__,task);
		
		thread_task_free(task);
	}
}

void thread_pool_destroy(thread_pool_t *tp)
{
	uint_t n;
	thread_task_t task;
	volatile uint_t lock;

	memset(&task, '\0', sizeof(thread_task_t));

	task.handler = thread_pool_exit_handler;
	task.ctx = (void*)&lock;

	for(n = 0; n < tp->threads; n++)
	{
		lock = 1;

		if(thread_task_post(tp, &task) != THREAD_POOL_OK)
		{
			return ;
		}

		while(lock)
		{
			sched_yield();
		}
	}

	(void)thread_cond_destroy(&tp->cond);
	(void)thread_mutex_destroy(&tp->mutex);

	free(tp);
}


static void 
thread_pool_exit_handler(void *data)
{
	uint_t *lock = (uint_t*)data;

	*lock = 0;

	pthread_exit(0);
}
