#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

/* gcc main.c thread_cond.c thread_mutex.c thread_pool.c -o thread_pool -lpthread */

typedef void (*handler)(void *data);

typedef struct argc_s {
	int id;
	char name[20];
}argc_t;

void task_handler1(void *data)
{
	printf("invok task_handler1....\n");
}

void task_handler2(void *data)
{
	printf("invok task_handler2....\n");
}

void task_handler3(void *data)
{
	printf("id %d invok task_handler3....\n", ((argc_t *)data)->id);
}

int main()
{
	int i = 0;
	printf("current thread: %ld\n", syscall(SYS_gettid));
	thread_pool_t *tp = thread_pool_create();
	if(tp == NULL)
	{
		fprintf(stderr, "thread pool create failed!\n");
		return -1;
	}

	for(;;){
		thread_task_t *task1 = thread_task_alloc(0);
		thread_task_t *task2 = thread_task_alloc(0);
		thread_task_t *task3 = thread_task_alloc(sizeof(argc_t));
		task1->handler = (handler)task_handler1;
		task2->handler = (handler)task_handler2;
		task3->handler = (handler)task_handler3;

		((argc_t*)(task3->ctx))->id = i;
		strcpy(((argc_t*)(task3->ctx))->name, "zhang san");
		
		thread_task_post(tp, task1);
		thread_task_post(tp, task2);
		thread_task_post(tp, task3);
		sleep(1);
		++i;
	}
	sleep(10);

	printf("destroy thread pool ...\n");
	thread_pool_destroy(tp);

	return 0;
}
