#include <stdio.h>

#include "algokit/queue.h"

/*
 * Context: demonstrate FIFO behavior by enqueueing integers and
 * dequeuing them in the original order. This models a simple task queue.
 */
int main(void) {
    int values[] = {4, 5, 6};
    size_t i;
    Item item;
    algokit_queue *queue = algokit_queue_create();

    /* Create the queue before inserting any items. */
    if (!queue) {
        return 1;
    }

    /* Enqueue each value so the first stays at the front. */
    for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        item.key = &values[i];
        item.val = NULL;
        if (algokit_queue_enqueue(queue, item) != 0) {
            algokit_queue_destroy(queue, NULL);
            return 1;
        }
    }

    /* Dequeue until empty to show FIFO order. */
    while (!algokit_queue_is_empty(queue)) {
        if (algokit_queue_dequeue(queue, &item) != 0) {
            algokit_queue_destroy(queue, NULL);
            return 1;
        }
        printf("%d\n", *(int *)item.key);
    }

    /* Destroy the queue nodes; user data stays untouched. */
    algokit_queue_destroy(queue, NULL);
    return 0;
}
