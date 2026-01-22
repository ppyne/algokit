#include "algokit/bst.h"

#include <stdlib.h>

struct algokit_bst_node {
    Item item;
    struct algokit_bst_node *left;
    struct algokit_bst_node *right;
};

struct algokit_bst {
    struct algokit_bst_node *root;
    algokit_key_cmp_fn cmp;
};

static int algokit_bst_cmp(const algokit_bst *tree, const void *a, const void *b) {
    return tree->cmp(a, b);
}

static struct algokit_bst_node *algokit_bst_node_create(Item item) {
    struct algokit_bst_node *node = (struct algokit_bst_node *)malloc(sizeof(*node));
    if (!node) {
        return NULL;
    }
    node->item = item;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void algokit_bst_node_destroy(struct algokit_bst_node *node, algokit_item_destroy_fn destroy) {
    if (!node) {
        return;
    }
    algokit_bst_node_destroy(node->left, destroy);
    algokit_bst_node_destroy(node->right, destroy);
    if (destroy) {
        destroy(node->item.key, node->item.val);
    }
    free(node);
}

static int algokit_bst_insert_node(struct algokit_bst *tree, struct algokit_bst_node **node, Item item) {
    int cmp;

    if (!*node) {
        *node = algokit_bst_node_create(item);
        return *node ? 0 : -1;
    }

    cmp = algokit_bst_cmp(tree, item.key, (*node)->item.key);
    if (cmp < 0) {
        return algokit_bst_insert_node(tree, &(*node)->left, item);
    }
    if (cmp > 0) {
        return algokit_bst_insert_node(tree, &(*node)->right, item);
    }

    return 1;
}

static int algokit_bst_search_node(const struct algokit_bst *tree, const struct algokit_bst_node *node, const void *key, Item *out) {
    if (!node) {
        return 1;
    }

    int cmp = algokit_bst_cmp(tree, key, node->item.key);
    if (cmp < 0) {
        return algokit_bst_search_node(tree, node->left, key, out);
    }
    if (cmp > 0) {
        return algokit_bst_search_node(tree, node->right, key, out);
    }

    *out = node->item;
    return 0;
}

static struct algokit_bst_node *algokit_bst_min_node(struct algokit_bst_node *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

static struct algokit_bst_node *algokit_bst_remove_node(struct algokit_bst *tree, struct algokit_bst_node *node, const void *key, Item *out, int *removed) {
    if (!node) {
        return NULL;
    }

    int cmp = algokit_bst_cmp(tree, key, node->item.key);
    if (cmp < 0) {
        node->left = algokit_bst_remove_node(tree, node->left, key, out, removed);
        return node;
    }
    if (cmp > 0) {
        node->right = algokit_bst_remove_node(tree, node->right, key, out, removed);
        return node;
    }

    *out = node->item;
    *removed = 1;

    if (!node->left) {
        struct algokit_bst_node *right = node->right;
        free(node);
        return right;
    }
    if (!node->right) {
        struct algokit_bst_node *left = node->left;
        free(node);
        return left;
    }

    struct algokit_bst_node *min = algokit_bst_min_node(node->right);
    node->item = min->item;
    {
        Item dummy_item;
        int dummy_removed = 0;
        node->right = algokit_bst_remove_node(tree, node->right, min->item.key, &dummy_item, &dummy_removed);
    }
    return node;
}

static int algokit_bst_traverse_node(const struct algokit_bst_node *node, algokit_bst_visit_fn visit, void *ctx, int order) {
    if (!node) {
        return 0;
    }

    if (order == 0) {
        visit(&node->item, ctx);
    }
    if (algokit_bst_traverse_node(node->left, visit, ctx, order) != 0) {
        return -1;
    }
    if (order == 1) {
        visit(&node->item, ctx);
    }
    if (algokit_bst_traverse_node(node->right, visit, ctx, order) != 0) {
        return -1;
    }
    if (order == 2) {
        visit(&node->item, ctx);
    }

    return 0;
}

algokit_bst *algokit_bst_create(algokit_key_cmp_fn cmp) {
    algokit_bst *tree;

    if (!cmp) {
        return NULL;
    }

    tree = (algokit_bst *)malloc(sizeof(*tree));
    if (!tree) {
        return NULL;
    }

    tree->root = NULL;
    tree->cmp = cmp;
    return tree;
}

void algokit_bst_destroy(algokit_bst *tree, algokit_item_destroy_fn destroy) {
    if (!tree) {
        return;
    }

    algokit_bst_node_destroy(tree->root, destroy);
    free(tree);
}

int algokit_bst_insert(algokit_bst *tree, Item item) {
    if (!tree) {
        return -1;
    }

    return algokit_bst_insert_node(tree, &tree->root, item);
}

int algokit_bst_search(const algokit_bst *tree, const void *key, Item *out) {
    if (!tree || !key || !out) {
        return -1;
    }

    return algokit_bst_search_node(tree, tree->root, key, out);
}

int algokit_bst_remove(algokit_bst *tree, const void *key, Item *out) {
    int removed = 0;

    if (!tree || !key || !out) {
        return -1;
    }

    tree->root = algokit_bst_remove_node(tree, tree->root, key, out, &removed);
    return removed ? 0 : 1;
}

int algokit_bst_traverse_inorder(const algokit_bst *tree, algokit_bst_visit_fn visit, void *ctx) {
    if (!tree || !visit) {
        return -1;
    }

    return algokit_bst_traverse_node(tree->root, visit, ctx, 1);
}

int algokit_bst_traverse_preorder(const algokit_bst *tree, algokit_bst_visit_fn visit, void *ctx) {
    if (!tree || !visit) {
        return -1;
    }

    return algokit_bst_traverse_node(tree->root, visit, ctx, 0);
}

int algokit_bst_traverse_postorder(const algokit_bst *tree, algokit_bst_visit_fn visit, void *ctx) {
    if (!tree || !visit) {
        return -1;
    }

    return algokit_bst_traverse_node(tree->root, visit, ctx, 2);
}
