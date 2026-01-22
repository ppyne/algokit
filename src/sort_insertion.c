#include "algokit/sort.h"

static int algokit_sort_cmp(const Item *a, const Item *b, algokit_key_cmp_fn cmp) {
    return cmp(a->key, b->key);
}

int algokit_sort_insertion(Item *items, size_t n, algokit_key_cmp_fn cmp) {
    size_t i;

    if (!cmp || (!items && n > 0)) {
        return -1;
    }

    for (i = 1; i < n; i++) {
        size_t j = i;
        Item tmp = items[i];
        while (j > 0 && algokit_sort_cmp(&items[j - 1], &tmp, cmp) > 0) {
            items[j] = items[j - 1];
            j--;
        }
        items[j] = tmp;
    }

    return 0;
}
