#include "algokit/sort.h"

#include <stddef.h>

static int algokit_sort_cmp(const Item *a, const Item *b, algokit_key_cmp_fn cmp) {
    return cmp(a->key, b->key);
}

static void algokit_sort_swap(Item *a, Item *b) {
    Item tmp = *a;
    *a = *b;
    *b = tmp;
}

static ptrdiff_t algokit_quick_partition(Item *items, ptrdiff_t low, ptrdiff_t high, algokit_key_cmp_fn cmp) {
    Item pivot = items[low + (high - low) / 2];
    ptrdiff_t i = low;
    ptrdiff_t j = high;

    while (1) {
        while (algokit_sort_cmp(&items[i], &pivot, cmp) < 0) {
            i++;
        }
        while (algokit_sort_cmp(&items[j], &pivot, cmp) > 0) {
            j--;
        }
        if (i >= j) {
            return j;
        }
        algokit_sort_swap(&items[i], &items[j]);
        i++;
        j--;
    }
}

static void algokit_quick_sort(Item *items, ptrdiff_t low, ptrdiff_t high, algokit_key_cmp_fn cmp) {
    if (low >= high) {
        return;
    }

    ptrdiff_t p = algokit_quick_partition(items, low, high, cmp);
    algokit_quick_sort(items, low, p, cmp);
    algokit_quick_sort(items, p + 1, high, cmp);
}

int algokit_sort_quick(Item *items, size_t n, algokit_key_cmp_fn cmp) {
    if (!cmp || (!items && n > 0)) {
        return -1;
    }

    if (n < 2) {
        return 0;
    }

    algokit_quick_sort(items, 0, (ptrdiff_t)n - 1, cmp);
    return 0;
}
