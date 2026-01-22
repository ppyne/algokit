#ifndef ALGOKIT_STACK_H
#define ALGOKIT_STACK_H

#include <stddef.h>
#include "algokit/item.h"

/*
 * Stack (LIFO)
 * Invariant: top points to most recently pushed item; size tracks node count.
 * Complexity: push/pop/peek O(1), size/is_empty O(1).
 * Ownership: stack owns only its nodes; user data freed via destroy callback.
 */

typedef struct algokit_stack algokit_stack;

algokit_stack *algokit_stack_create(void);

void algokit_stack_destroy(algokit_stack *stack, algokit_item_destroy_fn destroy);

int algokit_stack_push(algokit_stack *stack, Item item);

int algokit_stack_pop(algokit_stack *stack, Item *out);

int algokit_stack_peek(const algokit_stack *stack, Item *out);

size_t algokit_stack_size(const algokit_stack *stack);

int algokit_stack_is_empty(const algokit_stack *stack);

#endif /* ALGOKIT_STACK_H */
