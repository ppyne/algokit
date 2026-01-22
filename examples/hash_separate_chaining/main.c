#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algokit/hash.h"

/*
 * Context: store inventory counts keyed by integer product IDs.
 * We insert, retrieve, and delete entries using separate chaining.
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
    algokit_hash_sc *table = algokit_hash_sc_create(8, int_hash, int_cmp);
    Item item;
    int *key;
    int *count;

    /* Create the table before inserting any items. */
    if (!table) {
        return 1;
    }

    /* Insert a few product IDs and counts. */
    key = (int *)malloc(sizeof(*key));
    count = (int *)malloc(sizeof(*count));
    if (!key || !count) {
        free(key);
        free(count);
        algokit_hash_sc_destroy(table, destroy_kv);
        return 1;
    }
    *key = 100;
    *count = 7;
    item.key = key;
    item.val = count;
    if (algokit_hash_sc_put(table, item) != 0) {
        destroy_kv(item.key, item.val);
    }

    key = (int *)malloc(sizeof(*key));
    count = (int *)malloc(sizeof(*count));
    if (!key || !count) {
        free(key);
        free(count);
        algokit_hash_sc_destroy(table, destroy_kv);
        return 1;
    }
    *key = 200;
    *count = 3;
    item.key = key;
    item.val = count;
    if (algokit_hash_sc_put(table, item) != 0) {
        destroy_kv(item.key, item.val);
    }

    /* Retrieve an entry by key. */
    {
        int query = 200;
        if (algokit_hash_sc_get(table, &query, &item) == 0) {
            printf("get: %d -> %d\n", *(int *)item.key, *(int *)item.val);
        }
    }

    /* Delete an entry by key. */
    {
        int query = 100;
        if (algokit_hash_sc_delete(table, &query, &item) == 0) {
            printf("delete: %d -> %d\n", *(int *)item.key, *(int *)item.val);
            destroy_kv(item.key, item.val);
        }
    }

    /* Destroy table; remaining items freed by callback. */
    algokit_hash_sc_destroy(table, destroy_kv);
    return 0;
}
