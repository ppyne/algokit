#include <stdio.h>
#include <stdlib.h>

#include "algokit/rbtree.h"

/*
 * Context: store event IDs in an ordered log with balanced performance.
 * We insert records, search for one ID, and remove a record.
 */

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

    /* Insert a few event records. */
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
