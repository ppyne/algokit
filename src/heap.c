#include "algokit/heap.h"

#include <stdlib.h>

struct algokit_heap {
    Item *items;
    size_t size;
    size_t capacity;
    algokit_key_cmp_fn cmp;
};

static int algokit_heap_cmp(const algokit_heap *heap, const Item *a, const Item *b) {
    return heap->cmp(a->key, b->key);
}

static void algokit_heap_swap(Item *a, Item *b) {
    Item tmp = *a;
    *a = *b;
    *b = tmp;
}

static void algokit_heap_sift_up(algokit_heap *heap, size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) / 2;
        if (algokit_heap_cmp(heap, &heap->items[parent], &heap->items[idx]) >= 0) {
            return;
        }
        algokit_heap_swap(&heap->items[parent], &heap->items[idx]);
        idx = parent;
    }
}

static void algokit_heap_sift_down(algokit_heap *heap, size_t idx) {
    size_t n = heap->size;

    while (1) {
        size_t left = idx * 2 + 1;
        size_t right = left + 1;
        size_t largest = idx;

        if (left < n && algokit_heap_cmp(heap, &heap->items[left], &heap->items[largest]) > 0) {
            largest = left;
        }
        if (right < n && algokit_heap_cmp(heap, &heap->items[right], &heap->items[largest]) > 0) {
            largest = right;
        }
        if (largest == idx) {
            return;
        }
        algokit_heap_swap(&heap->items[idx], &heap->items[largest]);
        idx = largest;
    }
}

static int algokit_heap_grow(algokit_heap *heap) {
    size_t new_capacity = heap->capacity ? heap->capacity * 2 : 8;
    Item *new_items = (Item *)realloc(heap->items, new_capacity * sizeof(*new_items));

    if (!new_items) {
        return -1;
    }

    heap->items = new_items;
    heap->capacity = new_capacity;
    return 0;
}

algokit_heap *algokit_heap_create(size_t capacity, algokit_key_cmp_fn cmp) {
    algokit_heap *heap;

    if (!cmp) {
        return NULL;
    }

    heap = (algokit_heap *)malloc(sizeof(*heap));
    if (!heap) {
        return NULL;
    }

    heap->size = 0;
    heap->capacity = capacity;
    heap->cmp = cmp;
    heap->items = NULL;

    if (capacity > 0) {
        heap->items = (Item *)malloc(capacity * sizeof(*heap->items));
        if (!heap->items) {
            free(heap);
            return NULL;
        }
    }

    return heap;
}

void algokit_heap_destroy(algokit_heap *heap, algokit_item_destroy_fn destroy) {
    size_t i;

    if (!heap) {
        return;
    }

    if (destroy) {
        for (i = 0; i < heap->size; i++) {
            destroy(heap->items[i].key, heap->items[i].val);
        }
    }

    free(heap->items);
    free(heap);
}

int algokit_heap_insert(algokit_heap *heap, Item item) {
    if (!heap) {
        return -1;
    }

    if (heap->size == heap->capacity) {
        if (algokit_heap_grow(heap) != 0) {
            return -1;
        }
    }

    heap->items[heap->size] = item;
    algokit_heap_sift_up(heap, heap->size);
    heap->size++;
    return 0;
}

int algokit_heap_extract(algokit_heap *heap, Item *out) {
    if (!heap || !out || heap->size == 0) {
        return -1;
    }

    *out = heap->items[0];
    heap->size--;
    if (heap->size > 0) {
        heap->items[0] = heap->items[heap->size];
        algokit_heap_sift_down(heap, 0);
    }

    return 0;
}

int algokit_heap_peek(const algokit_heap *heap, Item *out) {
    if (!heap || !out || heap->size == 0) {
        return -1;
    }

    *out = heap->items[0];
    return 0;
}

size_t algokit_heap_size(const algokit_heap *heap) {
    if (!heap) {
        return 0;
    }

    return heap->size;
}

int algokit_heap_is_empty(const algokit_heap *heap) {
    return !heap || heap->size == 0;
}
