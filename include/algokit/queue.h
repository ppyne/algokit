#ifndef ALGOKIT_QUEUE_H
#define ALGOKIT_QUEUE_H

#include <stddef.h>
#include "algokit/item.h"

/*
 * Queue (FIFO)
 * Invariant: head is oldest item, tail is newest; size tracks node count.
 * Complexity: enqueue/dequeue/peek O(1), size/is_empty O(1).
 * Ownership: queue owns only its nodes; user data freed via destroy callback.
 */

typedef struct algokit_queue algokit_queue;

algokit_queue *algokit_queue_create(void);

void algokit_queue_destroy(algokit_queue *queue, algokit_item_destroy_fn destroy);

int algokit_queue_enqueue(algokit_queue *queue, Item item);

int algokit_queue_dequeue(algokit_queue *queue, Item *out);

int algokit_queue_peek(const algokit_queue *queue, Item *out);

size_t algokit_queue_size(const algokit_queue *queue);

int algokit_queue_is_empty(const algokit_queue *queue);

#endif /* ALGOKIT_QUEUE_H */
