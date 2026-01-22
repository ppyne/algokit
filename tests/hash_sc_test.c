#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "algokit/hash.h"

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
    int *val;
    int query;

    printf("hash_sc_test: create\n");
    assert(table != NULL);

    key = (int *)malloc(sizeof(*key));
    val = (int *)malloc(sizeof(*val));
    assert(key && val);
    *key = 3;
    *val = 33;
    item.key = key;
    item.val = val;
    assert(algokit_hash_sc_put(table, item) == 0);

    key = (int *)malloc(sizeof(*key));
    val = (int *)malloc(sizeof(*val));
    assert(key && val);
    *key = 4;
    *val = 44;
    item.key = key;
    item.val = val;
    assert(algokit_hash_sc_put(table, item) == 0);

    printf("hash_sc_test: get\n");
    query = 4;
    assert(algokit_hash_sc_get(table, &query, &item) == 0);
    assert(*(int *)item.val == 44);

    printf("hash_sc_test: delete\n");
    query = 3;
    assert(algokit_hash_sc_delete(table, &query, &item) == 0);
    assert(*(int *)item.val == 33);
    destroy_kv(item.key, item.val);

    printf("hash_sc_test: destroy\n");
    algokit_hash_sc_destroy(table, destroy_kv);
    printf("hash_sc_test: ok\n");
    return 0;
}
