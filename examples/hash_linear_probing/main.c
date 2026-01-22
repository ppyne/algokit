#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algokit/hash.h"

/*
 * Context: store user emails keyed by integer IDs using linear probing.
 * We insert, retrieve, and delete entries while keeping ownership clear.
 */

static size_t int_hash(const void *key) {
    return (size_t)(*(const int *)key * 2654435761u);
}

static int int_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void destroy_kv(void *key, void *val) {
    free(key);
    free(val);
}

int main(void) {
    algokit_hash_lp *table = algokit_hash_lp_create(8, int_hash, int_cmp);
    Item item;
    int *key;
    char *val;

    /* Create the table before inserting any items. */
    if (!table) {
        return 1;
    }

    /* Insert a few user IDs and emails. */
    key = (int *)malloc(sizeof(*key));
    val = (char *)malloc(16);
    if (!key || !val) {
        free(key);
        free(val);
        algokit_hash_lp_destroy(table, destroy_kv);
        return 1;
    }
    *key = 10;
    strcpy(val, "a@ex.com");
    item.key = key;
    item.val = val;
    if (algokit_hash_lp_put(table, item) != 0) {
        destroy_kv(item.key, item.val);
    }

    key = (int *)malloc(sizeof(*key));
    val = (char *)malloc(16);
    if (!key || !val) {
        free(key);
        free(val);
        algokit_hash_lp_destroy(table, destroy_kv);
        return 1;
    }
    *key = 20;
    strcpy(val, "b@ex.com");
    item.key = key;
    item.val = val;
    if (algokit_hash_lp_put(table, item) != 0) {
        destroy_kv(item.key, item.val);
    }

    /* Retrieve an entry by key. */
    {
        int query = 20;
        if (algokit_hash_lp_get(table, &query, &item) == 0) {
            printf("get: %d -> %s\n", *(int *)item.key, (char *)item.val);
        }
    }

    /* Delete an entry by key. */
    {
        int query = 10;
        if (algokit_hash_lp_delete(table, &query, &item) == 0) {
            printf("delete: %d -> %s\n", *(int *)item.key, (char *)item.val);
            destroy_kv(item.key, item.val);
        }
    }

    /* Destroy table; remaining items freed by callback. */
    algokit_hash_lp_destroy(table, destroy_kv);
    return 0;
}
