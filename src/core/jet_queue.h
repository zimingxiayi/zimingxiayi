#ifndef _JET_QUEUE_H_INCLUDED_
#define _JET_QUEUE_H_INCLUDED_

typedef signed int                                  jet_int_t;
typedef unsigned char                            u_char;
typedef struct jet_queue_s         jet_queue_t;

struct jet_queue_s {
    jet_queue_t* prev;
    jet_queue_t* next;
};


#define jet_queue_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q


#define jet_queue_empty(h)                                                    \
    (h == (h)->prev)


#define jet_queue_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x


#define jet_queue_insert_after   jet_queue_insert_head


#define jet_queue_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x


#define jet_queue_head(h)                                                     \
    (h)->next


#define jet_queue_last(h)                                                     \
    (h)->prev


#define jet_queue_sentinel(h)                                                 \
    (h)


#define jet_queue_next(q)                                                     \
    (q)->next


#define jet_queue_prev(q)                                                     \
    (q)->prev


#if (jet_DEBUG)

#define jet_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL

#else

#define jet_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif


#define jet_queue_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;


#define jet_queue_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;


#define jet_queue_data(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


jet_queue_t* jet_queue_middle(jet_queue_t* queue);
void jet_queue_sort(jet_queue_t* queue,
    jet_int_t(*cmp)(const jet_queue_t*, const jet_queue_t*));


#endif /* _JET_QUEUE_H_INCLUDED_ */
