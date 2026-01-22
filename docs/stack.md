# Stack (LIFO)

A stack is a LIFO (last in, first out) structure: the last element pushed is the first one removed. Think of a stack of plates.

## When to use it

- Undo actions
- Evaluate expressions
- Depth-first traversals

## What algokit provides

The stack stores `Item` values (from `algokit/item.h`), where each `Item` has a `void *key` and a `void *val`. The stack owns only its internal nodes. Your code owns `key` and `val`.

### Functions, parameters, and behavior

- `algokit_stack_create(void)`
  - Creates an empty stack.
  - Returns: a pointer to `algokit_stack`, or `NULL` on allocation failure.

- `algokit_stack_destroy(algokit_stack *stack, algokit_item_destroy_fn destroy)`
  - Frees all internal nodes and the stack itself.
  - Parameters:
    - `stack`: the stack to destroy (may be `NULL`).
    - `destroy`: optional callback to free `key`/`val`. If `NULL`, user data is left untouched.
  - Use this when you want the stack to clean up user data that you allocated.

- `algokit_stack_push(algokit_stack *stack, Item item)`
  - Pushes an `Item` to the top of the stack.
  - Parameters:
    - `stack`: the stack to modify (must not be `NULL`).
    - `item`: the `Item` to store (contains `key` and `val` pointers).
  - Returns: `0` on success, nonzero on error (for example, allocation failure).

- `algokit_stack_pop(algokit_stack *stack, Item *out)`
  - Removes the top item and writes it to `*out`.
  - Parameters:
    - `stack`: the stack to modify (must not be `NULL`).
    - `out`: output pointer receiving the removed `Item` (must not be `NULL`).
  - Returns: `0` on success, nonzero on error (for example, empty stack).

- `algokit_stack_peek(const algokit_stack *stack, Item *out)`
  - Reads the top item without removing it.
  - Parameters:
    - `stack`: the stack to read (must not be `NULL`).
    - `out`: output pointer receiving the top `Item` (must not be `NULL`).
  - Returns: `0` on success, nonzero on error (for example, empty stack).

- `algokit_stack_size(const algokit_stack *stack)`
  - Returns the number of elements in the stack.
  - If `stack` is `NULL`, returns `0`.

- `algokit_stack_is_empty(const algokit_stack *stack)`
  - Returns nonzero if the stack is empty, `0` otherwise.
  - If `stack` is `NULL`, returns nonzero.

### Limits and constraints

- The stack does not copy your `key` or `val`. Pointers stored must remain valid while the item is in the stack.
- The stack does not free your `key` or `val` unless you pass a `destroy` callback to `algokit_stack_destroy`.

## Key points

- The stack never frees your data by default.
  - Illustration: call `algokit_stack_destroy(stack, NULL)` to free only internal nodes.
- Pass a destroy callback to free `key`/`val`.
  - Illustration: pass `destroy_kv` to `algokit_stack_destroy` so every `key` and `val` is released.
- Stored pointers must remain valid while in the stack.
  - Illustration: store pointers to allocated strings or variables with a lifetime longer than the stack usage.

## Example covering all capabilities

Context: This example models a tiny configuration stack where each entry has a string key and value.
The data is allocated with `strdup` to simulate real configuration values coming from user input or files.
We want to push entries, inspect the top, then pop everything while freeing memory safely.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algokit/stack.h"

static void destroy_kv(void *key, void *val) {
    free(key);
    free(val);
}

int main(void) {
    Item item;
    algokit_stack *stack = algokit_stack_create();

    /* Create the stack before inserting any items. */
    if (!stack) {
        return 1;
    }

    /* Push a first key/value pair. */
    item.key = strdup("user_id");
    item.val = strdup("42");
    if (algokit_stack_push(stack, item) != 0) {
        destroy_kv(item.key, item.val);
        algokit_stack_destroy(stack, NULL);
        return 1;
    }

    /* Push a second key/value pair to be on top. */
    item.key = strdup("role");
    item.val = strdup("admin");
    if (algokit_stack_push(stack, item) != 0) {
        destroy_kv(item.key, item.val);
        algokit_stack_destroy(stack, destroy_kv);
        return 1;
    }

    /* Report how many items are stored. */
    printf("size = %zu\n", algokit_stack_size(stack));

    /* Peek at the top without removing it. */
    if (algokit_stack_peek(stack, &item) == 0) {
        printf("peek: %s = %s\n", (char *)item.key, (char *)item.val);
    }

    /* Pop items until the stack is empty, freeing each pair. */
    while (!algokit_stack_is_empty(stack)) {
        if (algokit_stack_pop(stack, &item) == 0) {
            printf("pop: %s = %s\n", (char *)item.key, (char *)item.val);
            destroy_kv(item.key, item.val);
        }
    }

    /* Destroy the stack nodes; no user data remains. */
    algokit_stack_destroy(stack, destroy_kv);
    return 0;
}
```
