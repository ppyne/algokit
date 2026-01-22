#include <stdio.h>

#include "algokit/sort.h"

/*
 * Context: sort a medium list of IDs where a faster method than insertion
 * is preferred but we still want an in-place algorithm.
 */

static int int_key_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void print_items(const Item *items, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%d:%s\n", *(int *)items[i].key, (const char *)items[i].val);
    }
}

int main(void) {
    int keys[] = {12, 7, 19, 3, 5, 14};
    const char *labels[] = {"l", "g", "s", "c", "e", "n"};
    Item items[6];
    size_t i;

    /* Build the Item array so each key travels with its label. */
    for (i = 0; i < 6; i++) {
        items[i].key = &keys[i];
        items[i].val = (void *)labels[i];
    }

    /* Show the original order. */
    printf("before:\n");
    print_items(items, 6);

    /* Sort in-place with shell sort using the key comparator. */
    if (algokit_sort_shell(items, 6, int_key_cmp) != 0) {
        return 1;
    }

    /* Show the sorted result. */
    printf("after:\n");
    print_items(items, 6);

    return 0;
}
