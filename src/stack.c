#include "algokit/stack.h"

#include <stdlib.h>

struct algokit_stack_node {
    Item item;
    struct algokit_stack_node *next;
};

struct algokit_stack {
    struct algokit_stack_node *top;
    size_t size;
};

algokit_stack *algokit_stack_create(void) {
    algokit_stack *stack = (algokit_stack *)malloc(sizeof(*stack));
    if (!stack) {
        return NULL;
    }
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

void algokit_stack_destroy(algokit_stack *stack, algokit_item_destroy_fn destroy) {
    struct algokit_stack_node *node;

    if (!stack) {
        return;
    }

    node = stack->top;
    while (node) {
        struct algokit_stack_node *next = node->next;
        if (destroy) {
            destroy(node->item.key, node->item.val);
        }
        free(node);
        node = next;
    }

    free(stack);
}

int algokit_stack_push(algokit_stack *stack, Item item) {
    struct algokit_stack_node *node;

    if (!stack) {
        return -1;
    }

    node = (struct algokit_stack_node *)malloc(sizeof(*node));
    if (!node) {
        return -1;
    }

    node->item = item;
    node->next = stack->top;
    stack->top = node;
    stack->size++;

    return 0;
}

int algokit_stack_pop(algokit_stack *stack, Item *out) {
    struct algokit_stack_node *node;

    if (!stack || !out || !stack->top) {
        return -1;
    }

    node = stack->top;
    *out = node->item;
    stack->top = node->next;
    stack->size--;
    free(node);

    return 0;
}

int algokit_stack_peek(const algokit_stack *stack, Item *out) {
    if (!stack || !out || !stack->top) {
        return -1;
    }

    *out = stack->top->item;
    return 0;
}

size_t algokit_stack_size(const algokit_stack *stack) {
    if (!stack) {
        return 0;
    }

    return stack->size;
}

int algokit_stack_is_empty(const algokit_stack *stack) {
    return !stack || stack->size == 0;
}
