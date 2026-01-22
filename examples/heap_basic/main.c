#include <stdio.h>
#include <stdlib.h>

#include "algokit/heap.h"

/*
 * Context: schedule tasks by priority. Higher priority should be extracted first.
 * We store integer priorities as keys and short labels as values.
 */

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

    /* Insert a low-priority task. */
    priority = (int *)malloc(sizeof(*priority));
    label = (char *)malloc(8);
    if (!priority || !label) {
        free(priority);
        free(label);
        algokit_heap_destroy(heap, destroy_kv);
        return 1;
    }
    *priority = 2;
    label[0] = 'l'; label[1] = 'o'; label[2] = 'w'; label[3] = '\0';
    item.key = priority;
    item.val = label;
    if (algokit_heap_insert(heap, item) != 0) {
        destroy_kv(item.key, item.val);
        algokit_heap_destroy(heap, destroy_kv);
        return 1;
    }

    /* Insert a high-priority task. */
    priority = (int *)malloc(sizeof(*priority));
    label = (char *)malloc(8);
    if (!priority || !label) {
        free(priority);
        free(label);
        algokit_heap_destroy(heap, destroy_kv);
        return 1;
    }
    *priority = 9;
    label[0] = 'h'; label[1] = 'i'; label[2] = 'g'; label[3] = 'h'; label[4] = '\0';
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
