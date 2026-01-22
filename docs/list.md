# Singly linked list

A linked list is a sequence of elements connected by pointers. You can access the front quickly, but not a random index.

## When to use it

- Build a simple queue or stack
- Insert often at the front
- Manage a stream of items without random access

## What algokit provides

The list stores `Item` values (from `algokit/item.h`), where each `Item` has a `void *key` and a `void *val`. The list owns only its internal nodes. Your code owns `key` and `val`.

### Functions, parameters, and behavior

- `algokit_list_create(void)`
  - Creates an empty list.
  - Returns: a pointer to `algokit_list`, or `NULL` on allocation failure.

- `algokit_list_destroy(algokit_list *list, algokit_item_destroy_fn destroy)`
  - Frees all internal nodes and the list itself.
  - Parameters:
    - `list`: the list to destroy (may be `NULL`).
    - `destroy`: optional callback to free `key`/`val`. If `NULL`, user data is left untouched.

- `algokit_list_push_front(algokit_list *list, Item item)`
  - Adds an `Item` to the front.
  - Parameters:
    - `list`: the list to modify (must not be `NULL`).
    - `item`: the `Item` to store (contains `key` and `val` pointers).
  - Returns: `0` on success, nonzero on error (for example, allocation failure).

- `algokit_list_push_back(algokit_list *list, Item item)`
  - Adds an `Item` to the back.
  - Parameters:
    - `list`: the list to modify (must not be `NULL`).
    - `item`: the `Item` to store (contains `key` and `val` pointers).
  - Returns: `0` on success, nonzero on error (for example, allocation failure).

- `algokit_list_pop_front(algokit_list *list, Item *out)`
  - Removes the front item and writes it to `*out`.
  - Parameters:
    - `list`: the list to modify (must not be `NULL`).
    - `out`: output pointer receiving the removed `Item` (must not be `NULL`).
  - Returns: `0` on success, nonzero on error (for example, empty list).

- `algokit_list_peek_front(const algokit_list *list, Item *out)`
  - Reads the front item without removing it.
  - Parameters:
    - `list`: the list to read (must not be `NULL`).
    - `out`: output pointer receiving the front `Item` (must not be `NULL`).
  - Returns: `0` on success, nonzero on error (for example, empty list).

- `algokit_list_size(const algokit_list *list)`
  - Returns the number of elements in the list.
  - If `list` is `NULL`, returns `0`.

- `algokit_list_is_empty(const algokit_list *list)`
  - Returns nonzero if the list is empty, `0` otherwise.
  - If `list` is `NULL`, returns nonzero.

### Limits and constraints

- The list does not copy your `key` or `val`. Pointers stored must remain valid while the item is in the list.
- This is a singly linked list with front operations only; there is no random access.
- The list does not free your `key` or `val` unless you pass a `destroy` callback to `algokit_list_destroy`.

## Key points

- The list never frees your data by default.
  - Illustration: call `algokit_list_destroy(list, NULL)` to free only internal nodes.
- Pass a destroy callback to free `key`/`val`.
  - Illustration: pass `destroy_kv` to `algokit_list_destroy` so every `key` and `val` is released.
- Stored pointers must remain valid while in the list.
  - Illustration: store pointers to allocated strings or variables with a lifetime longer than the list usage.

## Example covering all capabilities

Context: This example models a small address book that stores pairs like city and country.
We allocate strings to represent user-provided data, insert at both front and back, then read and remove items.
We want to demonstrate each list operation and show safe memory cleanup.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algokit/list.h"

static void destroy_kv(void *key, void *val) {
    free(key);
    free(val);
}

int main(void) {
    Item item;
    algokit_list *list = algokit_list_create();

    /* Create the list before inserting any items. */
    if (!list) {
        return 1;
    }

    /* Insert one item at the front. */
    item.key = strdup("city");
    item.val = strdup("Paris");
    if (algokit_list_push_front(list, item) != 0) {
        destroy_kv(item.key, item.val);
        algokit_list_destroy(list, NULL);
        return 1;
    }

    /* Insert another item at the back. */
    item.key = strdup("country");
    item.val = strdup("France");
    if (algokit_list_push_back(list, item) != 0) {
        destroy_kv(item.key, item.val);
        algokit_list_destroy(list, destroy_kv);
        return 1;
    }

    /* Report how many items are stored. */
    printf("size = %zu\n", algokit_list_size(list));

    /* Peek at the front without removing it. */
    if (algokit_list_peek_front(list, &item) == 0) {
        printf("peek: %s = %s\n", (char *)item.key, (char *)item.val);
    }

    /* Pop items until the list is empty, freeing each pair. */
    while (!algokit_list_is_empty(list)) {
        if (algokit_list_pop_front(list, &item) == 0) {
            printf("pop: %s = %s\n", (char *)item.key, (char *)item.val);
            destroy_kv(item.key, item.val);
        }
    }

    /* Destroy the list nodes; no user data remains. */
    algokit_list_destroy(list, destroy_kv);
    return 0;
}
```
