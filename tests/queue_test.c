#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "algokit/queue.h"

int main(void) {
    enum { kCount = 1000 };
    int values[kCount];
    size_t i;
    Item item;
    algokit_queue *queue = algokit_queue_create();

    printf("queue_test: create\n");
    assert(queue != NULL);
    assert(algokit_queue_is_empty(queue));
    assert(algokit_queue_size(queue) == 0);

    printf("queue_test: enqueue %d items\n", kCount);
    for (i = 0; i < kCount; i++) {
        values[i] = (int)i;
        item.key = &values[i];
        item.val = NULL;
        assert(algokit_queue_enqueue(queue, item) == 0);
    }

    assert(algokit_queue_size(queue) == kCount);

    printf("queue_test: dequeue %d items\n", kCount);
    for (i = 0; i < kCount; i++) {
        assert(algokit_queue_dequeue(queue, &item) == 0);
        assert(item.key == &values[i]);
    }

    assert(algokit_queue_is_empty(queue));
    assert(algokit_queue_size(queue) == 0);

    printf("queue_test: destroy\n");
    algokit_queue_destroy(queue, NULL);
    printf("queue_test: ok\n");
    return 0;
}
