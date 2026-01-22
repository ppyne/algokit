#include "algokit/sort.h"

static int algokit_sort_cmp(const Item *a, const Item *b, algokit_key_cmp_fn cmp) {
    return cmp(a->key, b->key);
}

static void algokit_sort_swap(Item *a, Item *b) {
    Item tmp = *a;
    *a = *b;
    *b = tmp;
}

static void algokit_heap_sift_down(Item *items, size_t start, size_t end, algokit_key_cmp_fn cmp) {
    size_t root = start;

    while (1) {
        size_t left = root * 2 + 1;
        size_t right = left + 1;
        size_t swap_idx = root;

        if (left < end && algokit_sort_cmp(&items[swap_idx], &items[left], cmp) < 0) {
            swap_idx = left;
        }
        if (right < end && algokit_sort_cmp(&items[swap_idx], &items[right], cmp) < 0) {
            swap_idx = right;
        }
        if (swap_idx == root) {
            return;
        }
        algokit_sort_swap(&items[root], &items[swap_idx]);
        root = swap_idx;
    }
}

int algokit_sort_heap(Item *items, size_t n, algokit_key_cmp_fn cmp) {
    size_t i;
    size_t end;

    if (!cmp || (!items && n > 0)) {
        return -1;
    }

    if (n < 2) {
        return 0;
    }

    for (i = n / 2; i > 0; i--) {
        algokit_heap_sift_down(items, i - 1, n, cmp);
    }

    for (end = n; end > 1; end--) {
        algokit_sort_swap(&items[0], &items[end - 1]);
        algokit_heap_sift_down(items, 0, end - 1, cmp);
    }

    return 0;
}
