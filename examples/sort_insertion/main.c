#include <stdio.h>

#include "algokit/sort.h"

/*
 * Context: sort small batches of incoming scores where simplicity matters.
 * We store integer scores as keys and short labels as values.
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
    int keys[] = {5, 1, 4, 2, 3};
    const char *labels[] = {"e", "a", "d", "b", "c"};
    Item items[5];
    size_t i;

    /* Build the Item array so each key travels with its label. */
    for (i = 0; i < 5; i++) {
        items[i].key = &keys[i];
        items[i].val = (void *)labels[i];
    }

    /* Show the original order. */
    printf("before:\n");
    print_items(items, 5);

    /* Sort in-place with insertion sort using the key comparator. */
    if (algokit_sort_insertion(items, 5, int_key_cmp) != 0) {
        return 1;
    }

    /* Show the sorted result. */
    printf("after:\n");
    print_items(items, 5);

    return 0;
}
