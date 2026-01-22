#ifndef ALGOKIT_RBTREE_H
#define ALGOKIT_RBTREE_H

#include "algokit/item.h"

/*
 * Red-Black Tree (left-leaning)
 * Invariant: red links lean left and no node has two red links.
 * Ownership: tree owns only its nodes; user data freed via destroy callback.
 */

typedef struct algokit_rbtree algokit_rbtree;

typedef void (*algokit_rbtree_visit_fn)(const Item *item, void *ctx);

algokit_rbtree *algokit_rbtree_create(algokit_key_cmp_fn cmp);

void algokit_rbtree_destroy(algokit_rbtree *tree, algokit_item_destroy_fn destroy);

int algokit_rbtree_insert(algokit_rbtree *tree, Item item);

int algokit_rbtree_search(const algokit_rbtree *tree, const void *key, Item *out);

int algokit_rbtree_remove(algokit_rbtree *tree, const void *key, Item *out);

int algokit_rbtree_traverse_inorder(const algokit_rbtree *tree, algokit_rbtree_visit_fn visit, void *ctx);

int algokit_rbtree_traverse_preorder(const algokit_rbtree *tree, algokit_rbtree_visit_fn visit, void *ctx);

int algokit_rbtree_traverse_postorder(const algokit_rbtree *tree, algokit_rbtree_visit_fn visit, void *ctx);

#endif /* ALGOKIT_RBTREE_H */
