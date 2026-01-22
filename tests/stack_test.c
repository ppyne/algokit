#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "algokit/stack.h"

int main(void) {
    enum { kCount = 1000 };
    int values[kCount];
    size_t i;
    Item item;
    algokit_stack *stack = algokit_stack_create();

    printf("stack_test: create\n");
    assert(stack != NULL);
    assert(algokit_stack_is_empty(stack));
    assert(algokit_stack_size(stack) == 0);

    printf("stack_test: push %d items\n", kCount);
    for (i = 0; i < kCount; i++) {
        values[i] = (int)i;
        item.key = &values[i];
        item.val = NULL;
        assert(algokit_stack_push(stack, item) == 0);
    }

    assert(algokit_stack_size(stack) == kCount);

    printf("stack_test: pop %d items\n", kCount);
    for (i = 0; i < kCount; i++) {
        assert(algokit_stack_pop(stack, &item) == 0);
        assert(item.key == &values[kCount - 1 - i]);
    }

    assert(algokit_stack_is_empty(stack));
    assert(algokit_stack_size(stack) == 0);

    printf("stack_test: destroy\n");
    algokit_stack_destroy(stack, NULL);
    printf("stack_test: ok\n");
    return 0;
}
