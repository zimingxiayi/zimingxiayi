#include "jet_queue.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LENGTH   15

#define _jet_srand() srand((unsigned)time(0))
#define _jet_malloc(type) (type*)malloc(sizeof(type))
#define _jet_free(__ptr) if(__ptr) free(__ptr)

typedef struct jet_data_queue_s jet_data_queue_t;
struct jet_data_queue_s {
	int          data;
	jet_queue_t queue;
};

#define jet_queue_data_offset(q, type) \
	((type*)(((u_char*)(q)) - (sizeof(type) - sizeof(jet_queue_t))))

#define jet_queue_data_insert(q, n) \
	(q)->data = n

void insert_head_qeueu(jet_queue_t *jq, jet_data_queue_t *tq, int data)
{
        jet_queue_data_insert(tq, data);
	jet_queue_insert_head(jq, &tq->queue);
}

void insert_tail_qeueu(jet_queue_t *jq, jet_data_queue_t *tq, int data)
{
	jet_queue_data_insert(tq, data);
	jet_queue_insert_tail(jq, &tq->queue);
}

void remove_data_queue(jet_queue_t *jq)
{
	jet_data_queue_t *node = jet_queue_data(jq, jet_data_queue_t, queue);
	jet_queue_remove(&node->queue);
	printf("free(%p)\n", node);
	_jet_free(node);
}

void delete_data_queue(jet_queue_t *jq, jet_int_t pos)
{
	jet_int_t index = 0;
	jet_queue_t *q = 0;
        for(q = jet_queue_head(jq); q != jet_queue_sentinel(jq); q = jet_queue_next(q))
        {
		if(pos == index)
		{
                	remove_data_queue(q);
		}
		++index;
        }
}

void destroy_queue(jet_queue_t *jq)
{
	jet_queue_t *q = 0;
        for(q = jet_queue_head(jq); q != jet_queue_sentinel(jq); q = jet_queue_next(q))
        {
                remove_data_queue(q);
        }
}

void print_queue(jet_queue_t *jq)
{
	jet_queue_t *q = 0;
	for(q = jet_queue_head(jq); q != jet_queue_sentinel(jq); q = jet_queue_next(q))
	{
		jet_data_queue_t* node = jet_queue_data(q, jet_data_queue_t, queue);
		printf("%d ", node->data);
	}
	printf("\n");
}

jet_int_t compore(const jet_queue_t *left, const jet_queue_t *right)
{
	jet_data_queue_t *lq = jet_queue_data(left, jet_data_queue_t, queue);
	jet_data_queue_t *rq = jet_queue_data(right, jet_data_queue_t, queue);

	return lq->data > rq->data;
}

#define jet_address(addr) printf("%s: %p\n", #addr, addr)

int main(int argc, char* argv[])
{
	_jet_srand();
	jet_queue_t *jq = _jet_malloc(jet_queue_t);
	jet_data_queue_t* tq = 0;
	jet_queue_init(jq);

	int index = 0;
	printf("insert queue:\n");
	for(index = 0; index < MAX_LENGTH; ++index)
	{
		tq = _jet_malloc(jet_data_queue_t);
		jet_address(tq);
		insert_head_qeueu(jq, tq, rand() % MAX_LENGTH);
	}

	print_queue(jq);

	printf("sort queue:\n");	
	jet_queue_sort(jq, compore);

	print_queue(jq);

	printf("delete queue:\n");	
	delete_data_queue(jq, 3);
	print_queue(jq);

	printf("destroy queue:\n");
	destroy_queue(jq);

	jet_queue_t *q = 0;
	for(q = jet_queue_head(jq); q != jet_queue_sentinel(jq); q = jet_queue_next(q))
	{
		remove_data_queue(q);
	}
	
	return 0;
}
