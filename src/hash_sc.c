#include "algokit/hash.h"

#include <stdlib.h>

struct algokit_hash_sc_node {
    Item item;
    struct algokit_hash_sc_node *next;
};

struct algokit_hash_sc {
    struct algokit_hash_sc_node **buckets;
    size_t capacity;
    size_t size;
    algokit_key_hash_fn hash;
    algokit_key_cmp_fn cmp;
};

static size_t algokit_hash_sc_index(const algokit_hash_sc *table, const void *key) {
    return table->hash(key) % table->capacity;
}

static struct algokit_hash_sc_node *algokit_hash_sc_node_create(Item item) {
    struct algokit_hash_sc_node *node = (struct algokit_hash_sc_node *)malloc(sizeof(*node));
    if (!node) {
        return NULL;
    }
    node->item = item;
    node->next = NULL;
    return node;
}

static int algokit_hash_sc_resize(algokit_hash_sc *table, size_t new_capacity) {
    size_t i;
    struct algokit_hash_sc_node **old_buckets = table->buckets;
    size_t old_capacity = table->capacity;

    table->buckets = (struct algokit_hash_sc_node **)calloc(new_capacity, sizeof(*table->buckets));
    if (!table->buckets) {
        table->buckets = old_buckets;
        return -1;
    }

    table->capacity = new_capacity;
    table->size = 0;

    for (i = 0; i < old_capacity; i++) {
        struct algokit_hash_sc_node *node = old_buckets[i];
        while (node) {
            struct algokit_hash_sc_node *next = node->next;
            node->next = NULL;
            algokit_hash_sc_put(table, node->item);
            free(node);
            node = next;
        }
    }

    free(old_buckets);
    return 0;
}

algokit_hash_sc *algokit_hash_sc_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp) {
    algokit_hash_sc *table;

    if (!hash || !cmp || capacity == 0) {
        return NULL;
    }

    table = (algokit_hash_sc *)malloc(sizeof(*table));
    if (!table) {
        return NULL;
    }

    table->buckets = (struct algokit_hash_sc_node **)calloc(capacity, sizeof(*table->buckets));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    table->capacity = capacity;
    table->size = 0;
    table->hash = hash;
    table->cmp = cmp;
    return table;
}

void algokit_hash_sc_destroy(algokit_hash_sc *table, algokit_item_destroy_fn destroy) {
    size_t i;

    if (!table) {
        return;
    }

    for (i = 0; i < table->capacity; i++) {
        struct algokit_hash_sc_node *node = table->buckets[i];
        while (node) {
            struct algokit_hash_sc_node *next = node->next;
            if (destroy) {
                destroy(node->item.key, node->item.val);
            }
            free(node);
            node = next;
        }
    }

    free(table->buckets);
    free(table);
}

int algokit_hash_sc_put(algokit_hash_sc *table, Item item) {
    size_t idx;
    struct algokit_hash_sc_node *node;

    if (!table) {
        return -1;
    }

    if ((table->size + 1) > table->capacity * 2) {
        if (algokit_hash_sc_resize(table, table->capacity * 2) != 0) {
            return -1;
        }
    }

    idx = algokit_hash_sc_index(table, item.key);
    node = table->buckets[idx];
    while (node) {
        if (table->cmp(node->item.key, item.key) == 0) {
            return 1;
        }
        node = node->next;
    }

    node = algokit_hash_sc_node_create(item);
    if (!node) {
        return -1;
    }

    node->next = table->buckets[idx];
    table->buckets[idx] = node;
    table->size++;
    return 0;
}

int algokit_hash_sc_get(const algokit_hash_sc *table, const void *key, Item *out) {
    size_t idx;
    struct algokit_hash_sc_node *node;

    if (!table || !key || !out) {
        return -1;
    }

    idx = algokit_hash_sc_index(table, key);
    node = table->buckets[idx];
    while (node) {
        if (table->cmp(node->item.key, key) == 0) {
            *out = node->item;
            return 0;
        }
        node = node->next;
    }

    return 1;
}

int algokit_hash_sc_delete(algokit_hash_sc *table, const void *key, Item *out) {
    size_t idx;
    struct algokit_hash_sc_node *node;
    struct algokit_hash_sc_node *prev = NULL;

    if (!table || !key || !out) {
        return -1;
    }

    idx = algokit_hash_sc_index(table, key);
    node = table->buckets[idx];
    while (node) {
        if (table->cmp(node->item.key, key) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                table->buckets[idx] = node->next;
            }
            *out = node->item;
            free(node);
            table->size--;
            if (table->capacity > 8 && table->size < table->capacity / 4) {
                algokit_hash_sc_resize(table, table->capacity / 2);
            }
            return 0;
        }
        prev = node;
        node = node->next;
    }

    return 1;
}
