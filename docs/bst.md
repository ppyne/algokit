# Binary Search Tree (BST)

A binary search tree stores key/value pairs in a sorted structure. For any node, all keys in the left subtree are smaller and all keys in the right subtree are larger (according to the comparison function).

## When to use it

- Ordered dictionaries or symbol tables
- Range queries by key order
- In-order traversal to get sorted output

## What algokit provides

The BST stores `Item` values and orders them by `Item.key` using a user-provided comparison function.

### Functions, parameters, and behavior

- `algokit_bst_create(algokit_key_cmp_fn cmp)`
  - Creates an empty BST.
  - Parameters: `cmp` must not be `NULL`.
  - Returns: `algokit_bst *` or `NULL` on failure.

- `algokit_bst_destroy(algokit_bst *tree, algokit_item_destroy_fn destroy)`
  - Frees all nodes and the tree itself.
  - Parameters:
    - `tree`: tree to destroy (may be `NULL`).
    - `destroy`: optional callback to free `key`/`val` for each item.

- `algokit_bst_insert(algokit_bst *tree, Item item)`
  - Inserts a new item if the key does not already exist.
  - Parameters: `tree` must not be `NULL`.
  - Returns: `0` on success, `1` if the key already exists, nonzero on error.

- `algokit_bst_search(const algokit_bst *tree, const void *key, Item *out)`
  - Searches for a key and writes the found item to `*out`.
  - Parameters: `tree`, `key`, and `out` must not be `NULL`.
  - Returns: `0` on success, nonzero if not found or invalid parameters.

- `algokit_bst_remove(algokit_bst *tree, const void *key, Item *out)`
  - Removes the item with the given key and writes it to `*out`.
  - Parameters: `tree`, `key`, and `out` must not be `NULL`.
  - Returns: `0` on success, nonzero if not found or invalid parameters.

- `algokit_bst_traverse_inorder(const algokit_bst *tree, algokit_bst_visit_fn visit, void *ctx)`
  - Visits nodes in ascending key order.
  - Parameters: `tree` and `visit` must not be `NULL`.
  - Returns: `0` on success, nonzero on error.

- `algokit_bst_traverse_preorder(const algokit_bst *tree, algokit_bst_visit_fn visit, void *ctx)`
  - Visits each node before its subtrees.
  - Parameters: `tree` and `visit` must not be `NULL`.
  - Returns: `0` on success, nonzero on error.

- `algokit_bst_traverse_postorder(const algokit_bst *tree, algokit_bst_visit_fn visit, void *ctx)`
  - Visits each node after its subtrees.
  - Parameters: `tree` and `visit` must not be `NULL`.
  - Returns: `0` on success, nonzero on error.

### Limits and constraints

- Keys must remain valid while stored in the tree.
- Duplicate keys are rejected (insert returns `1`).
- The BST does not free user data unless a destroy callback is provided.

## Key points

- In-order traversal yields sorted keys.
  - Illustration: `algokit_bst_traverse_inorder` visits keys in ascending order.
- Search/insert/remove depend on the shape of the tree.
  - Illustration: a skewed tree degrades to linear time.
- The comparison function defines ordering; the tree never inspects key types.
  - Illustration: you can store strings, integers, or structs via a custom comparator.

## Example covering all capabilities

Context: store book IDs in a catalog. We insert a few IDs with labels, search for one,
traverse in order to list them, remove an ID, then traverse again.

```c
#include <stdio.h>
#include <stdlib.h>
#include "algokit/bst.h"

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
    algokit_bst *tree = algokit_bst_create(int_key_cmp);
    Item item;
    int *key;
    char *label;

    /* Create the tree before inserting any items. */
    if (!tree) {
        return 1;
    }

    /* Insert a few items. */
    key = (int *)malloc(sizeof(*key));
    label = (char *)malloc(6);
    if (!key || !label) {
        free(key);
        free(label);
        algokit_bst_destroy(tree, destroy_kv);
        return 1;
    }
    *key = 3;
    label[0] = 'b'; label[1] = 'o'; label[2] = 'o'; label[3] = 'k'; label[4] = '3'; label[5] = '\0';
    item.key = key;
    item.val = label;
    if (algokit_bst_insert(tree, item) != 0) {
        destroy_kv(item.key, item.val);
    }

    key = (int *)malloc(sizeof(*key));
    label = (char *)malloc(6);
    if (!key || !label) {
        free(key);
        free(label);
        algokit_bst_destroy(tree, destroy_kv);
        return 1;
    }
    *key = 1;
    label[0] = 'b'; label[1] = 'o'; label[2] = 'o'; label[3] = 'k'; label[4] = '1'; label[5] = '\0';
    item.key = key;
    item.val = label;
    if (algokit_bst_insert(tree, item) != 0) {
        destroy_kv(item.key, item.val);
    }

    key = (int *)malloc(sizeof(*key));
    label = (char *)malloc(6);
    if (!key || !label) {
        free(key);
        free(label);
        algokit_bst_destroy(tree, destroy_kv);
        return 1;
    }
    *key = 4;
    label[0] = 'b'; label[1] = 'o'; label[2] = 'o'; label[3] = 'k'; label[4] = '4'; label[5] = '\0';
    item.key = key;
    item.val = label;
    if (algokit_bst_insert(tree, item) != 0) {
        destroy_kv(item.key, item.val);
    }

    /* Search for a specific book ID. */
    {
        int query = 1;
        if (algokit_bst_search(tree, &query, &item) == 0) {
            printf("found: %d:%s\n", *(int *)item.key, (char *)item.val);
        }
    }

    /* Traverse in order to list books by ID. */
    printf("inorder:\n");
    algokit_bst_traverse_inorder(tree, print_item, NULL);

    /* Remove a book by ID. */
    {
        int query = 3;
        if (algokit_bst_remove(tree, &query, &item) == 0) {
            printf("removed: %d:%s\n", *(int *)item.key, (char *)item.val);
            destroy_kv(item.key, item.val);
        }
    }

    /* Traverse again after removal. */
    printf("inorder after removal:\n");
    algokit_bst_traverse_inorder(tree, print_item, NULL);

    /* Destroy the tree and any remaining items. */
    algokit_bst_destroy(tree, destroy_kv);
    return 0;
}
```
