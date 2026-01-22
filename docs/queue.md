# Queue (FIFO)

A queue is FIFO (first in, first out): the first element enqueued is the first one removed. Think of people waiting in line.

## When to use it

- Buffers (producer/consumer)
- Simple scheduling (tasks)
- Processing in arrival order

## What algokit provides

The queue stores `Item` values (from `algokit/item.h`), where each `Item` has a `void *key` and a `void *val`. The queue owns only its internal nodes. Your code owns `key` and `val`.

### Functions, parameters, and behavior

- `algokit_queue_create(void)`
  - Creates an empty queue.
  - Returns: a pointer to `algokit_queue`, or `NULL` on allocation failure.

- `algokit_queue_destroy(algokit_queue *queue, algokit_item_destroy_fn destroy)`
  - Frees all internal nodes and the queue itself.
  - Parameters:
    - `queue`: the queue to destroy (may be `NULL`).
    - `destroy`: optional callback to free `key`/`val`. If `NULL`, user data is left untouched.

- `algokit_queue_enqueue(algokit_queue *queue, Item item)`
  - Adds an `Item` to the back of the queue.
  - Parameters:
    - `queue`: the queue to modify (must not be `NULL`).
    - `item`: the `Item` to store (contains `key` and `val` pointers).
  - Returns: `0` on success, nonzero on error (for example, allocation failure).

- `algokit_queue_dequeue(algokit_queue *queue, Item *out)`
  - Removes the front item and writes it to `*out`.
  - Parameters:
    - `queue`: the queue to modify (must not be `NULL`).
    - `out`: output pointer receiving the removed `Item` (must not be `NULL`).
  - Returns: `0` on success, nonzero on error (for example, empty queue).

- `algokit_queue_peek(const algokit_queue *queue, Item *out)`
  - Reads the front item without removing it.
  - Parameters:
    - `queue`: the queue to read (must not be `NULL`).
    - `out`: output pointer receiving the front `Item` (must not be `NULL`).
  - Returns: `0` on success, nonzero on error (for example, empty queue).

- `algokit_queue_size(const algokit_queue *queue)`
  - Returns the number of elements in the queue.
  - If `queue` is `NULL`, returns `0`.

- `algokit_queue_is_empty(const algokit_queue *queue)`
  - Returns nonzero if the queue is empty, `0` otherwise.
  - If `queue` is `NULL`, returns nonzero.

### Limits and constraints

- The queue does not copy your `key` or `val`. Pointers stored must remain valid while the item is in the queue.
- The queue does not free your `key` or `val` unless you pass a `destroy` callback to `algokit_queue_destroy`.

## Key points

- The queue never frees your data by default.
  - Illustration: call `algokit_queue_destroy(queue, NULL)` to free only internal nodes.
- Pass a destroy callback to free `key`/`val`.
  - Illustration: pass `destroy_kv` to `algokit_queue_destroy` so every `key` and `val` is released.
- Stored pointers must remain valid while in the queue.
  - Illustration: store pointers to allocated strings or variables with a lifetime longer than the queue usage.

## Example covering all capabilities

Context: This example models a simple job queue where each entry has a job type and a payload.
We allocate strings to simulate data coming from outside the program, then process them in arrival order.
We want to enqueue jobs, inspect the next job, then dequeue and free each entry safely.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algokit/queue.h"

static void destroy_kv(void *key, void *val) {
    free(key);
    free(val);
}

int main(void) {
    Item item;
    algokit_queue *queue = algokit_queue_create();

    /* Create the queue before inserting any items. */
    if (!queue) {
        return 1;
    }

    /* Enqueue a first job. */
    item.key = strdup("task");
    item.val = strdup("email");
    if (algokit_queue_enqueue(queue, item) != 0) {
        destroy_kv(item.key, item.val);
        algokit_queue_destroy(queue, NULL);
        return 1;
    }

    /* Enqueue a second job that will run after the first. */
    item.key = strdup("task");
    item.val = strdup("report");
    if (algokit_queue_enqueue(queue, item) != 0) {
        destroy_kv(item.key, item.val);
        algokit_queue_destroy(queue, destroy_kv);
        return 1;
    }

    /* Report how many jobs are queued. */
    printf("size = %zu\n", algokit_queue_size(queue));

    /* Peek at the next job without removing it. */
    if (algokit_queue_peek(queue, &item) == 0) {
        printf("peek: %s = %s\n", (char *)item.key, (char *)item.val);
    }

    /* Dequeue jobs until the queue is empty, freeing each pair. */
    while (!algokit_queue_is_empty(queue)) {
        if (algokit_queue_dequeue(queue, &item) == 0) {
            printf("dequeue: %s = %s\n", (char *)item.key, (char *)item.val);
            destroy_kv(item.key, item.val);
        }
    }

    /* Destroy the queue nodes; no user data remains. */
    algokit_queue_destroy(queue, destroy_kv);
    return 0;
}
```
