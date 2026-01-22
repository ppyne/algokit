#include "algokit/sort.h"

#include <stdlib.h>

static int algokit_sort_cmp(const Item *a, const Item *b, algokit_key_cmp_fn cmp) {
    return cmp(a->key, b->key);
}

static void algokit_merge(Item *items, Item *tmp, size_t left, size_t mid, size_t right, algokit_key_cmp_fn cmp) {
    size_t i = left;
    size_t j = mid;
    size_t k = left;

    while (i < mid && j < right) {
        if (algokit_sort_cmp(&items[i], &items[j], cmp) <= 0) {
            tmp[k++] = items[i++];
        } else {
            tmp[k++] = items[j++];
        }
    }

    while (i < mid) {
        tmp[k++] = items[i++];
    }

    while (j < right) {
        tmp[k++] = items[j++];
    }

    for (i = left; i < right; i++) {
        items[i] = tmp[i];
    }
}

static void algokit_merge_sort(Item *items, Item *tmp, size_t left, size_t right, algokit_key_cmp_fn cmp) {
    size_t mid;

    if (right - left < 2) {
        return;
    }

    mid = left + (right - left) / 2;
    algokit_merge_sort(items, tmp, left, mid, cmp);
    algokit_merge_sort(items, tmp, mid, right, cmp);
    algokit_merge(items, tmp, left, mid, right, cmp);
}

int algokit_sort_merge(Item *items, size_t n, algokit_key_cmp_fn cmp) {
    Item *tmp;

    if (!cmp || (!items && n > 0)) {
        return -1;
    }

    if (n < 2) {
        return 0;
    }

    tmp = (Item *)malloc(n * sizeof(*tmp));
    if (!tmp) {
        return -1;
    }

    algokit_merge_sort(items, tmp, 0, n, cmp);
    free(tmp);
    return 0;
}
