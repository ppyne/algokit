# Red-Black Tree (left-leaning)

A red-black tree is a balanced binary search tree. It keeps the tree height near log(n) by coloring links red or black and enforcing balance rules.

## When to use it

- Ordered dictionaries with predictable performance
- Large datasets where a plain BST might become skewed
- Fast inserts and deletes with balanced height

## What algokit provides

The red-black tree stores `Item` values and orders them by `Item.key` using a user-provided comparison function. This implementation uses left-leaning red-black trees.

### Functions, parameters, and behavior

- `algokit_rbtree_create(algokit_key_cmp_fn cmp)`
  - Creates an empty red-black tree.
  - Parameters: `cmp` must not be `NULL`.
  - Returns: `algokit_rbtree *` or `NULL` on failure.

- `algokit_rbtree_destroy(algokit_rbtree *tree, algokit_item_destroy_fn destroy)`
  - Frees all nodes and the tree itself.
  - Parameters:
    - `tree`: tree to destroy (may be `NULL`).
    - `destroy`: optional callback to free `key`/`val` for each item.

- `algokit_rbtree_insert(algokit_rbtree *tree, Item item)`
  - Inserts a new item if the key does not already exist.
  - Parameters: `tree` must not be `NULL`.
  - Returns: `0` on success, `1` if the key already exists, nonzero on error.

- `algokit_rbtree_search(const algokit_rbtree *tree, const void *key, Item *out)`
  - Searches for a key and writes the found item to `*out`.
  - Parameters: `tree`, `key`, and `out` must not be `NULL`.
  - Returns: `0` on success, nonzero if not found or invalid parameters.

- `algokit_rbtree_remove(algokit_rbtree *tree, const void *key, Item *out)`
  - Removes the item with the given key and writes it to `*out`.
  - Parameters: `tree`, `key`, and `out` must not be `NULL`.
  - Returns: `0` on success, nonzero if not found or invalid parameters.

- `algokit_rbtree_traverse_inorder(const algokit_rbtree *tree, algokit_rbtree_visit_fn visit, void *ctx)`
  - Visits nodes in ascending key order.
  - Parameters: `tree` and `visit` must not be `NULL`.
  - Returns: `0` on success, nonzero on error.

- `algokit_rbtree_traverse_preorder(const algokit_rbtree *tree, algokit_rbtree_visit_fn visit, void *ctx)`
  - Visits each node before its subtrees.
  - Parameters: `tree` and `visit` must not be `NULL`.
  - Returns: `0` on success, nonzero on error.

- `algokit_rbtree_traverse_postorder(const algokit_rbtree *tree, algokit_rbtree_visit_fn visit, void *ctx)`
  - Visits each node after its subtrees.
  - Parameters: `tree` and `visit` must not be `NULL`.
  - Returns: `0` on success, nonzero on error.

### Limits and constraints

- Keys must remain valid while stored in the tree.
- Duplicate keys are rejected (insert returns `1`).
- The tree does not free user data unless a destroy callback is provided.

## Key points

- The tree stays balanced using red/black coloring.
  - Illustration: insert and remove keep height near log(n).
- In-order traversal yields sorted keys.
  - Illustration: `algokit_rbtree_traverse_inorder` visits keys in ascending order.
- The comparison function defines ordering; the tree never inspects key types.
  - Illustration: you can store strings, integers, or structs via a custom comparator.

## Example covering all capabilities

Context: store event IDs in an ordered log. We insert a few IDs, search for one,
traverse in order to list them, remove an ID, then traverse again.

```c
#include <stdio.h>
#include <stdlib.h>
#include "algokit/rbtree.h"

static int int_key_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void destroy_kv(void *key, void *val) {
    free(key);
    free(val);
}

static void print_item(const Item *item, void *ctx) {
    (void)ctx;
    printf("%d:%s\n", *(int *)item->key, (char *)item->val);
}

int main(void) {
    algokit_rbtree *tree = algokit_rbtree_create(int_key_cmp);
    Item item;
    int *key;
    char *label;

    /* Create the tree before inserting any items. */
    if (!tree) {
        return 1;
    }

    /* Insert a few items. */
    key = (int *)malloc(sizeof(*key));
    label = (char *)malloc(7);
    if (!key || !label) {
        free(key);
        free(label);
        algokit_rbtree_destroy(tree, destroy_kv);
        return 1;
    }
    *key = 2;
    label[0] = 'e'; label[1] = 'v'; label[2] = 'e'; label[3] = 'n'; label[4] = 't'; label[5] = '2'; label[6] = '\0';
    item.key = key;
    item.val = label;
    if (algokit_rbtree_insert(tree, item) != 0) {
        destroy_kv(item.key, item.val);
    }

    key = (int *)malloc(sizeof(*key));
    label = (char *)malloc(7);
    if (!key || !label) {
        free(key);
        free(label);
        algokit_rbtree_destroy(tree, destroy_kv);
        return 1;
    }
    *key = 5;
    label[0] = 'e'; label[1] = 'v'; label[2] = 'e'; label[3] = 'n'; label[4] = 't'; label[5] = '5'; label[6] = '\0';
    item.key = key;
    item.val = label;
    if (algokit_rbtree_insert(tree, item) != 0) {
        destroy_kv(item.key, item.val);
    }

    key = (int *)malloc(sizeof(*key));
    label = (char *)malloc(7);
    if (!key || !label) {
        free(key);
        free(label);
        algokit_rbtree_destroy(tree, destroy_kv);
        return 1;
    }
    *key = 1;
    label[0] = 'e'; label[1] = 'v'; label[2] = 'e'; label[3] = 'n'; label[4] = 't'; label[5] = '1'; label[6] = '\0';
    item.key = key;
    item.val = label;
    if (algokit_rbtree_insert(tree, item) != 0) {
        destroy_kv(item.key, item.val);
    }

    /* Search for a specific event ID. */
    {
        int query = 5;
        if (algokit_rbtree_search(tree, &query, &item) == 0) {
            printf("found: %d:%s\n", *(int *)item.key, (char *)item.val);
        }
    }

    /* Traverse in order to list events by ID. */
    printf("inorder:\n");
    algokit_rbtree_traverse_inorder(tree, print_item, NULL);

    /* Remove an event by ID. */
    {
        int query = 2;
        if (algokit_rbtree_remove(tree, &query, &item) == 0) {
            printf("removed: %d:%s\n", *(int *)item.key, (char *)item.val);
            destroy_kv(item.key, item.val);
        }
    }

    /* Traverse again after removal. */
    printf("inorder after removal:\n");
    algokit_rbtree_traverse_inorder(tree, print_item, NULL);

    /* Destroy the tree and any remaining items. */
    algokit_rbtree_destroy(tree, destroy_kv);
    return 0;
}
```
