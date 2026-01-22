#include "algokit/rbtree.h"

#include <stdlib.h>

struct algokit_rbtree_node {
    Item item;
    struct algokit_rbtree_node *left;
    struct algokit_rbtree_node *right;
    int red;
};

struct algokit_rbtree {
    struct algokit_rbtree_node *root;
    algokit_key_cmp_fn cmp;
};

static int algokit_rbtree_cmp(const algokit_rbtree *tree, const void *a, const void *b) {
    return tree->cmp(a, b);
}

static int algokit_rbtree_is_red(const struct algokit_rbtree_node *node) {
    return node && node->red;
}

static struct algokit_rbtree_node *algokit_rbtree_node_create(Item item) {
    struct algokit_rbtree_node *node = (struct algokit_rbtree_node *)malloc(sizeof(*node));
    if (!node) {
        return NULL;
    }
    node->item = item;
    node->left = NULL;
    node->right = NULL;
    node->red = 1;
    return node;
}

static void algokit_rbtree_node_destroy(struct algokit_rbtree_node *node, algokit_item_destroy_fn destroy) {
    if (!node) {
        return;
    }
    algokit_rbtree_node_destroy(node->left, destroy);
    algokit_rbtree_node_destroy(node->right, destroy);
    if (destroy) {
        destroy(node->item.key, node->item.val);
    }
    free(node);
}

static struct algokit_rbtree_node *algokit_rbtree_rotate_left(struct algokit_rbtree_node *h) {
    struct algokit_rbtree_node *x = h->right;
    h->right = x->left;
    x->left = h;
    x->red = h->red;
    h->red = 1;
    return x;
}

static struct algokit_rbtree_node *algokit_rbtree_rotate_right(struct algokit_rbtree_node *h) {
    struct algokit_rbtree_node *x = h->left;
    h->left = x->right;
    x->right = h;
    x->red = h->red;
    h->red = 1;
    return x;
}

static void algokit_rbtree_flip_colors(struct algokit_rbtree_node *h) {
    h->red = !h->red;
    if (h->left) {
        h->left->red = !h->left->red;
    }
    if (h->right) {
        h->right->red = !h->right->red;
    }
}

static struct algokit_rbtree_node *algokit_rbtree_fix_up(struct algokit_rbtree_node *h) {
    if (algokit_rbtree_is_red(h->right) && !algokit_rbtree_is_red(h->left)) {
        h = algokit_rbtree_rotate_left(h);
    }
    if (algokit_rbtree_is_red(h->left) && algokit_rbtree_is_red(h->left->left)) {
        h = algokit_rbtree_rotate_right(h);
    }
    if (algokit_rbtree_is_red(h->left) && algokit_rbtree_is_red(h->right)) {
        algokit_rbtree_flip_colors(h);
    }
    return h;
}

static struct algokit_rbtree_node *algokit_rbtree_move_red_left(struct algokit_rbtree_node *h) {
    algokit_rbtree_flip_colors(h);
    if (algokit_rbtree_is_red(h->right) && algokit_rbtree_is_red(h->right->left)) {
        h->right = algokit_rbtree_rotate_right(h->right);
        h = algokit_rbtree_rotate_left(h);
        algokit_rbtree_flip_colors(h);
    }
    return h;
}

static struct algokit_rbtree_node *algokit_rbtree_move_red_right(struct algokit_rbtree_node *h) {
    algokit_rbtree_flip_colors(h);
    if (algokit_rbtree_is_red(h->left) && algokit_rbtree_is_red(h->left->left)) {
        h = algokit_rbtree_rotate_right(h);
        algokit_rbtree_flip_colors(h);
    }
    return h;
}

static struct algokit_rbtree_node *algokit_rbtree_min_node(struct algokit_rbtree_node *h) {
    while (h && h->left) {
        h = h->left;
    }
    return h;
}

static struct algokit_rbtree_node *algokit_rbtree_delete_min(struct algokit_rbtree_node *h) {
    if (!h->left) {
        free(h);
        return NULL;
    }

    if (!algokit_rbtree_is_red(h->left) && !algokit_rbtree_is_red(h->left->left)) {
        h = algokit_rbtree_move_red_left(h);
    }

    h->left = algokit_rbtree_delete_min(h->left);
    return algokit_rbtree_fix_up(h);
}

static struct algokit_rbtree_node *algokit_rbtree_insert_node(algokit_rbtree *tree, struct algokit_rbtree_node *h, Item item, int *inserted) {
    int cmp;

    if (!h) {
        *inserted = 1;
        return algokit_rbtree_node_create(item);
    }

    cmp = algokit_rbtree_cmp(tree, item.key, h->item.key);
    if (cmp < 0) {
        h->left = algokit_rbtree_insert_node(tree, h->left, item, inserted);
    } else if (cmp > 0) {
        h->right = algokit_rbtree_insert_node(tree, h->right, item, inserted);
    } else {
        *inserted = 0;
    }

    h = algokit_rbtree_fix_up(h);
    return h;
}

static int algokit_rbtree_search_node(const algokit_rbtree *tree, const struct algokit_rbtree_node *node, const void *key, Item *out) {
    if (!node) {
        return 1;
    }

    int cmp = algokit_rbtree_cmp(tree, key, node->item.key);
    if (cmp < 0) {
        return algokit_rbtree_search_node(tree, node->left, key, out);
    }
    if (cmp > 0) {
        return algokit_rbtree_search_node(tree, node->right, key, out);
    }

    *out = node->item;
    return 0;
}

static struct algokit_rbtree_node *algokit_rbtree_remove_node(algokit_rbtree *tree, struct algokit_rbtree_node *h, const void *key, Item *out, int *removed) {
    if (!h) {
        return NULL;
    }

    if (algokit_rbtree_cmp(tree, key, h->item.key) < 0) {
        if (h->left && !algokit_rbtree_is_red(h->left) && !algokit_rbtree_is_red(h->left->left)) {
            h = algokit_rbtree_move_red_left(h);
        }
        h->left = algokit_rbtree_remove_node(tree, h->left, key, out, removed);
    } else {
        if (algokit_rbtree_is_red(h->left)) {
            h = algokit_rbtree_rotate_right(h);
        }
        if (algokit_rbtree_cmp(tree, key, h->item.key) == 0 && !h->right) {
            *out = h->item;
            *removed = 1;
            free(h);
            return NULL;
        }
        if (h->right && !algokit_rbtree_is_red(h->right) && !algokit_rbtree_is_red(h->right->left)) {
            h = algokit_rbtree_move_red_right(h);
        }
        if (algokit_rbtree_cmp(tree, key, h->item.key) == 0) {
            struct algokit_rbtree_node *min = algokit_rbtree_min_node(h->right);
            *out = h->item;
            *removed = 1;
            h->item = min->item;
            h->right = algokit_rbtree_delete_min(h->right);
        } else {
            h->right = algokit_rbtree_remove_node(tree, h->right, key, out, removed);
        }
    }

    return algokit_rbtree_fix_up(h);
}

static int algokit_rbtree_traverse_node(const struct algokit_rbtree_node *node, algokit_rbtree_visit_fn visit, void *ctx, int order) {
    if (!node) {
        return 0;
    }

    if (order == 0) {
        visit(&node->item, ctx);
    }
    if (algokit_rbtree_traverse_node(node->left, visit, ctx, order) != 0) {
        return -1;
    }
    if (order == 1) {
        visit(&node->item, ctx);
    }
    if (algokit_rbtree_traverse_node(node->right, visit, ctx, order) != 0) {
        return -1;
    }
    if (order == 2) {
        visit(&node->item, ctx);
    }

    return 0;
}

algokit_rbtree *algokit_rbtree_create(algokit_key_cmp_fn cmp) {
    algokit_rbtree *tree;

    if (!cmp) {
        return NULL;
    }

    tree = (algokit_rbtree *)malloc(sizeof(*tree));
    if (!tree) {
        return NULL;
    }

    tree->root = NULL;
    tree->cmp = cmp;
    return tree;
}

void algokit_rbtree_destroy(algokit_rbtree *tree, algokit_item_destroy_fn destroy) {
    if (!tree) {
        return;
    }

    algokit_rbtree_node_destroy(tree->root, destroy);
    free(tree);
}

int algokit_rbtree_insert(algokit_rbtree *tree, Item item) {
    int inserted = 0;

    if (!tree) {
        return -1;
    }

    tree->root = algokit_rbtree_insert_node(tree, tree->root, item, &inserted);
    if (!tree->root) {
        return -1;
    }
    tree->root->red = 0;

    return inserted ? 0 : 1;
}

int algokit_rbtree_search(const algokit_rbtree *tree, const void *key, Item *out) {
    if (!tree || !key || !out) {
        return -1;
    }

    return algokit_rbtree_search_node(tree, tree->root, key, out);
}

int algokit_rbtree_remove(algokit_rbtree *tree, const void *key, Item *out) {
    int removed = 0;

    if (!tree || !key || !out) {
        return -1;
    }

    if (!tree->root) {
        return 1;
    }

    tree->root = algokit_rbtree_remove_node(tree, tree->root, key, out, &removed);
    if (tree->root) {
        tree->root->red = 0;
    }

    return removed ? 0 : 1;
}

int algokit_rbtree_traverse_inorder(const algokit_rbtree *tree, algokit_rbtree_visit_fn visit, void *ctx) {
    if (!tree || !visit) {
        return -1;
    }

    return algokit_rbtree_traverse_node(tree->root, visit, ctx, 1);
}

int algokit_rbtree_traverse_preorder(const algokit_rbtree *tree, algokit_rbtree_visit_fn visit, void *ctx) {
    if (!tree || !visit) {
        return -1;
    }

    return algokit_rbtree_traverse_node(tree->root, visit, ctx, 0);
}

int algokit_rbtree_traverse_postorder(const algokit_rbtree *tree, algokit_rbtree_visit_fn visit, void *ctx) {
    if (!tree || !visit) {
        return -1;
    }

    return algokit_rbtree_traverse_node(tree->root, visit, ctx, 2);
}
