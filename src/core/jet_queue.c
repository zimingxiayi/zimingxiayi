#include "jet_queue.h"

jet_queue_t*
jet_queue_middle(jet_queue_t* queue)
{
    jet_queue_t* middle, * next;

    middle = jet_queue_head(queue);

    if (middle == jet_queue_last(queue)) {
        return middle;
    }

    next = jet_queue_head(queue);

    for (;; ) {
        middle = jet_queue_next(middle);

        next = jet_queue_next(next);

        if (next == jet_queue_last(queue)) {
            return middle;
        }

        next = jet_queue_next(next);

        if (next == jet_queue_last(queue)) {
            return middle;
        }
    }
}


/* the stable insertion sort */

void
jet_queue_sort(jet_queue_t* queue,
    jet_int_t(*cmp)(const jet_queue_t*, const jet_queue_t*))
{
    jet_queue_t* q, * prev, * next;

    q = jet_queue_head(queue);

    if (q == jet_queue_last(queue)) {
        return;
    }

    for (q = jet_queue_next(q); q != jet_queue_sentinel(queue); q = next) {

        prev = jet_queue_prev(q);
        next = jet_queue_next(q);

        jet_queue_remove(q);

        do {
            if (cmp(prev, q) <= 0) {
                break;
            }

            prev = jet_queue_prev(prev);

        } while (prev != jet_queue_sentinel(queue));

        jet_queue_insert_after(prev, q);
    }
}
