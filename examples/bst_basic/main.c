#include <stdio.h>
#include <stdlib.h>

#include "algokit/bst.h"

/*
 * Context: store book IDs in a catalog and list them in sorted order.
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
    algokit_bst *tree = algokit_bst_create(int_key_cmp);
    Item item;
    int *key;
    char *label;

    /* Create the tree before inserting any items. */
    if (!tree) {
        return 1;
    }

    /* Insert a few book records. */
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
