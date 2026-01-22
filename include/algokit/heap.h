#ifndef ALGOKIT_HEAP_H
#define ALGOKIT_HEAP_H

#include <stddef.h>
#include "algokit/item.h"

/*
 * Binary max-heap
 * Invariant: parent key >= child keys according to cmp.
 * Complexity: insert/extract O(log n), peek O(1), size/is_empty O(1).
 * Ownership: heap owns only its internal array; user data freed via destroy callback.
 */

typedef struct algokit_heap algokit_heap;

algokit_heap *algokit_heap_create(size_t capacity, algokit_key_cmp_fn cmp);

void algokit_heap_destroy(algokit_heap *heap, algokit_item_destroy_fn destroy);

int algokit_heap_insert(algokit_heap *heap, Item item);

int algokit_heap_extract(algokit_heap *heap, Item *out);

int algokit_heap_peek(const algokit_heap *heap, Item *out);

size_t algokit_heap_size(const algokit_heap *heap);

int algokit_heap_is_empty(const algokit_heap *heap);

#endif /* ALGOKIT_HEAP_H */
