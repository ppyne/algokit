# Binary heap (max-heap)

A binary heap is a complete tree stored in an array. A max-heap keeps the largest key at the top.

## When to use it

- Priority queues
- Scheduling tasks by priority
- Fast access to the current maximum

## What algokit provides

The heap stores `Item` values (from `algokit/item.h`) and orders them by `Item.key` using a comparison function.

### Functions, parameters, and behavior

- `algokit_heap_create(size_t capacity, algokit_key_cmp_fn cmp)`
  - Creates an empty max-heap with an initial capacity.
  - Parameters:
    - `capacity`: initial storage size (can be 0, heap grows automatically).
    - `cmp`: key comparison function (must not be `NULL`).
  - Returns: `algokit_heap *` or `NULL` on failure.

- `algokit_heap_destroy(algokit_heap *heap, algokit_item_destroy_fn destroy)`
  - Frees the heap array and the heap itself.
  - Parameters:
    - `heap`: heap to destroy (may be `NULL`).
    - `destroy`: optional callback to free `key`/`val` for each stored item.

- `algokit_heap_insert(algokit_heap *heap, Item item)`
  - Inserts an item and restores heap order.
  - Parameters: `heap` must not be `NULL`.
  - Returns: `0` on success, nonzero on error (e.g., allocation failure).

- `algokit_heap_extract(algokit_heap *heap, Item *out)`
  - Removes the maximum item and writes it to `*out`.
  - Parameters: `heap` and `out` must not be `NULL`.
  - Returns: `0` on success, nonzero on error (e.g., empty heap).

- `algokit_heap_peek(const algokit_heap *heap, Item *out)`
  - Reads the maximum item without removing it.
  - Parameters: `heap` and `out` must not be `NULL`.
  - Returns: `0` on success, nonzero on error (e.g., empty heap).

- `algokit_heap_size(const algokit_heap *heap)`
  - Returns the number of items in the heap (0 if `heap` is `NULL`).

- `algokit_heap_is_empty(const algokit_heap *heap)`
  - Returns nonzero if empty, `0` otherwise.

### Limits and constraints

- The heap does not copy `key` or `val`. Pointers stored must remain valid while in the heap.
- The heap does not free `key` or `val` unless a destroy callback is provided.

## Key points

- The heap always keeps the largest key at the top.
  - Illustration: `algokit_heap_peek` returns the maximum without removing it.
- Insert/extract are logarithmic time due to sift operations.
  - Illustration: `algokit_heap_insert` and `algokit_heap_extract` reposition elements as needed.
- The heap grows automatically if capacity is exceeded.
  - Illustration: inserting beyond capacity triggers internal resizing.

## Example covering all capabilities

Context: we schedule tasks by priority (higher number = higher priority). We insert tasks, peek at the top,
extract them in order, and free any allocated data.

```c
#include <stdio.h>
#include <stdlib.h>
#include "algokit/heap.h"

static int int_key_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void destroy_kv(void *key, void *val) {
    free(key);
    free(val);
}

int main(void) {
    algokit_heap *heap = algokit_heap_create(0, int_key_cmp);
    Item item;
    int *priority;
    char *label;

    /* Create the heap before inserting any items. */
    if (!heap) {
        return 1;
    }

    /* Insert a few tasks with priorities. */
    priority = (int *)malloc(sizeof(*priority));
    label = (char *)malloc(8);
    if (!priority || !label) {
        free(priority);
        free(label);
        algokit_heap_destroy(heap, destroy_kv);
        return 1;
    }
    *priority = 5;
    label[0] = 'a'; label[1] = 's'; label[2] = 's'; label[3] = 'i'; label[4] = 'g'; label[5] = 'n'; label[6] = '\0';
    item.key = priority;
    item.val = label;
    if (algokit_heap_insert(heap, item) != 0) {
        destroy_kv(item.key, item.val);
        algokit_heap_destroy(heap, destroy_kv);
        return 1;
    }

    priority = (int *)malloc(sizeof(*priority));
    label = (char *)malloc(8);
    if (!priority || !label) {
        free(priority);
        free(label);
        algokit_heap_destroy(heap, destroy_kv);
        return 1;
    }
    *priority = 9;
    label[0] = 'u'; label[1] = 'r'; label[2] = 'g'; label[3] = 'e'; label[4] = 'n'; label[5] = 't'; label[6] = '\0';
    item.key = priority;
    item.val = label;
    if (algokit_heap_insert(heap, item) != 0) {
        destroy_kv(item.key, item.val);
        algokit_heap_destroy(heap, destroy_kv);
        return 1;
    }

    /* Peek at the top priority task. */
    if (algokit_heap_peek(heap, &item) == 0) {
        printf("peek: %d %s\n", *(int *)item.key, (char *)item.val);
    }

    /* Extract tasks in descending priority order. */
    while (!algokit_heap_is_empty(heap)) {
        if (algokit_heap_extract(heap, &item) == 0) {
            printf("extract: %d %s\n", *(int *)item.key, (char *)item.val);
            destroy_kv(item.key, item.val);
        }
    }

    /* Destroy the heap array; user data already freed. */
    algokit_heap_destroy(heap, destroy_kv);
    return 0;
}
```
