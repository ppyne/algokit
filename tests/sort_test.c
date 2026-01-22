#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "algokit/sort.h"

static int int_key_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static int is_sorted(const Item *items, size_t n) {
    size_t i;
    for (i = 1; i < n; i++) {
        int prev = *(int *)items[i - 1].key;
        int curr = *(int *)items[i].key;
        if (prev > curr) {
            return 0;
        }
    }
    return 1;
}

static void copy_items(Item *dst, const Item *src, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

int main(void) {
    int keys[] = {5, 1, 4, 2, 3};
    Item original[5];
    Item work[5];
    size_t i;

    for (i = 0; i < 5; i++) {
        original[i].key = &keys[i];
        original[i].val = NULL;
    }

    printf("sort_test: insertion\n");
    copy_items(work, original, 5);
    assert(algokit_sort_insertion(work, 5, int_key_cmp) == 0);
    assert(is_sorted(work, 5));

    printf("sort_test: shell\n");
    copy_items(work, original, 5);
    assert(algokit_sort_shell(work, 5, int_key_cmp) == 0);
    assert(is_sorted(work, 5));

    printf("sort_test: quick\n");
    copy_items(work, original, 5);
    assert(algokit_sort_quick(work, 5, int_key_cmp) == 0);
    assert(is_sorted(work, 5));

    printf("sort_test: merge\n");
    copy_items(work, original, 5);
    assert(algokit_sort_merge(work, 5, int_key_cmp) == 0);
    assert(is_sorted(work, 5));

    printf("sort_test: heap\n");
    copy_items(work, original, 5);
    assert(algokit_sort_heap(work, 5, int_key_cmp) == 0);
    assert(is_sorted(work, 5));

    printf("sort_test: ok\n");
    return 0;
}
