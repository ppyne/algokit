#ifndef ALGOKIT_BST_H
#define ALGOKIT_BST_H

#include "algokit/item.h"

/*
 * Binary Search Tree (BST)
 * Invariant: left subtree keys < node key < right subtree keys (by cmp).
 * Ownership: tree owns only its nodes; user data freed via destroy callback.
 */

typedef struct algokit_bst algokit_bst;

typedef void (*algokit_bst_visit_fn)(const Item *item, void *ctx);

algokit_bst *algokit_bst_create(algokit_key_cmp_fn cmp);

void algokit_bst_destroy(algokit_bst *tree, algokit_item_destroy_fn destroy);

int algokit_bst_insert(algokit_bst *tree, Item item);

int algokit_bst_search(const algokit_bst *tree, const void *key, Item *out);

int algokit_bst_remove(algokit_bst *tree, const void *key, Item *out);

int algokit_bst_traverse_inorder(const algokit_bst *tree, algokit_bst_visit_fn visit, void *ctx);

int algokit_bst_traverse_preorder(const algokit_bst *tree, algokit_bst_visit_fn visit, void *ctx);

int algokit_bst_traverse_postorder(const algokit_bst *tree, algokit_bst_visit_fn visit, void *ctx);

#endif /* ALGOKIT_BST_H */
