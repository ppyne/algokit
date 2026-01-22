#include <stdio.h>

#include "algokit/stack.h"

/*
 * Context: demonstrate a basic LIFO workflow by pushing integers and popping
 * them in reverse order. This models undo-like behavior on simple values.
 */
int main(void) {
    int values[] = {1, 2, 3};
    size_t i;
    Item item;
    algokit_stack *stack = algokit_stack_create();

    /* Create the stack before inserting any items. */
    if (!stack) {
        return 1;
    }

    /* Push each value so the last one becomes the top. */
    for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        item.key = &values[i];
        item.val = NULL;
        if (algokit_stack_push(stack, item) != 0) {
            algokit_stack_destroy(stack, NULL);
            return 1;
        }
    }

    /* Pop until empty to show LIFO order. */
    while (!algokit_stack_is_empty(stack)) {
        if (algokit_stack_pop(stack, &item) != 0) {
            algokit_stack_destroy(stack, NULL);
            return 1;
        }
        printf("%d\n", *(int *)item.key);
    }

    /* Destroy the stack nodes; user data stays untouched. */
    algokit_stack_destroy(stack, NULL);
    return 0;
}
