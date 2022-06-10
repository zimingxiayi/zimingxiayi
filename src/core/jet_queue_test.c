#define _CRT_SECURE_NO_WARNINGS
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

void subscript_delete_queue(jet_queue_t *jq, jet_int_t pos)
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

void element_delete_queue(jet_queue_t *jq, jet_int_t value)
{
	jet_queue_t *q = 0;
    for(q = jet_queue_head(jq); q != jet_queue_sentinel(jq); q = jet_queue_next(q))
    {
		if(value == (jet_queue_data(q, jet_data_queue_t, queue))->data)
		{
            remove_data_queue(q);
		}
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

enum func
{
    add = 0,
    del_i, // index delete
    del_v, // value delete
    mod,   // modify
    fin,   // find
    sor,   // sort
    des,   // destroy
};

int main(int argc, char* argv[])
{
	_jet_srand();
    int index = 0;
    const char *space = "*               "; // 16
    jet_int_t choose = 0, run_flag = 1;
	jet_queue_t *jq = _jet_malloc(jet_queue_t);
	jet_data_queue_t* tq = 0;
	jet_queue_init(jq);

    while(run_flag)
    {
        printf("***********************************************************\n");
        printf("\
%s%d. New node element.\n\
%s%d. Index delete node element.\n\
%s%d. Value delete node element.\n\
%s%d. Modify node element.\n\
%s%d. Find node element.\n\
%s%d. Sort node element.\n\
%s%d. Destroy node element.\n\
%s[less %d or greater %d exit.]\n", 
                space, add,
                space, del_i,
                space, del_v,
                space, mod,
                space, fin,
                space, sor,
                space, des, 
                space, add, 
                des);
        printf("***********************************************************\n");
        scanf("%d", &choose);
        switch(choose)
        {
            case add:
	            for(index = 0; index < MAX_LENGTH; ++index)
	            {
		            tq = _jet_malloc(jet_data_queue_t);
		            jet_address(tq);
		            insert_head_qeueu(jq, tq, rand() % MAX_LENGTH);
	            }
                break;

            case del_i:
                printf("Please enter the subscript of the element to be delete: ");
                scanf("%d", &choose);
	            subscript_delete_queue(jq, choose);
                break;

            case del_v:
                printf("Please enter a vlaue to delete the element: ");
                scanf("%d", &choose);
	            element_delete_queue(jq, choose);
                break;
            
            case fin:
	            print_queue(jq);
                break;

            case sor:
                jet_queue_sort(jq, compore);
                break;

            case des:
	            destroy_queue(jq);
                break;

            default:
                run_flag = 0;
                break;
        }
    }
	
	return 0;
}
