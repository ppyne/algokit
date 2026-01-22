#include <assert.h>
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
    int prev;

    printf("heap_test: create\n");
    assert(heap != NULL);

    /* Insert a few priorities. */
    priority = (int *)malloc(sizeof(*priority));
    assert(priority != NULL);
    *priority = 3;
    item.key = priority;
    item.val = NULL;
    assert(algokit_heap_insert(heap, item) == 0);

    priority = (int *)malloc(sizeof(*priority));
    assert(priority != NULL);
    *priority = 8;
    item.key = priority;
    item.val = NULL;
    assert(algokit_heap_insert(heap, item) == 0);

    priority = (int *)malloc(sizeof(*priority));
    assert(priority != NULL);
    *priority = 1;
    item.key = priority;
    item.val = NULL;
    assert(algokit_heap_insert(heap, item) == 0);

    printf("heap_test: peek\n");
    assert(algokit_heap_peek(heap, &item) == 0);
    assert(*(int *)item.key == 8);

    printf("heap_test: extract\n");
    prev = 1000;
    while (!algokit_heap_is_empty(heap)) {
        assert(algokit_heap_extract(heap, &item) == 0);
        assert(*(int *)item.key <= prev);
        prev = *(int *)item.key;
        destroy_kv(item.key, item.val);
    }

    printf("heap_test: destroy\n");
    algokit_heap_destroy(heap, destroy_kv);
    printf("heap_test: ok\n");
    return 0;
}
