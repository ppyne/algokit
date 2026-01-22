#include <stdio.h>

#include "algokit/list.h"

/*
 * Context: demonstrate front/back insertion and front removal using a list.
 * This models a simple stream where new items can arrive at either end.
 */
int main(void) {
    int values[] = {7, 8, 9};
    size_t i;
    Item item;
    algokit_list *list = algokit_list_create();

    /* Create the list before inserting any items. */
    if (!list) {
        return 1;
    }

    /* Insert items at the back to preserve order. */
    for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        item.key = &values[i];
        item.val = NULL;
        if (algokit_list_push_back(list, item) != 0) {
            algokit_list_destroy(list, NULL);
            return 1;
        }
    }

    /* Pop from the front until empty. */
    while (!algokit_list_is_empty(list)) {
        if (algokit_list_pop_front(list, &item) != 0) {
            algokit_list_destroy(list, NULL);
            return 1;
        }
        printf("%d\n", *(int *)item.key);
    }

    /* Destroy the list nodes; user data stays untouched. */
    algokit_list_destroy(list, NULL);
    return 0;
}
