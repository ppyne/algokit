#include "algokit/sort.h"

static int algokit_sort_cmp(const Item *a, const Item *b, algokit_key_cmp_fn cmp) {
    return cmp(a->key, b->key);
}

int algokit_sort_shell(Item *items, size_t n, algokit_key_cmp_fn cmp) {
    size_t gap;
    size_t i;

    if (!cmp || (!items && n > 0)) {
        return -1;
    }

    gap = 1;
    while (gap < n / 3) {
        gap = 3 * gap + 1;
    }

    while (gap > 0) {
        for (i = gap; i < n; i++) {
            size_t j;
            Item tmp = items[i];
            for (j = i; j >= gap && algokit_sort_cmp(&items[j - gap], &tmp, cmp) > 0; j -= gap) {
                items[j] = items[j - gap];
            }
            items[j] = tmp;
        }
        gap /= 3;
    }

    return 0;
}
