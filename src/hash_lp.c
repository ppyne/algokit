#include "algokit/hash.h"

#include <stdlib.h>

struct algokit_hash_lp_entry {
    Item item;
    int state;
};

struct algokit_hash_lp {
    struct algokit_hash_lp_entry *entries;
    size_t capacity;
    size_t size;
    algokit_key_hash_fn hash;
    algokit_key_cmp_fn cmp;
};

static size_t algokit_hash_lp_index(const algokit_hash_lp *table, const void *key) {
    return table->hash(key) % table->capacity;
}

static int algokit_hash_lp_resize(algokit_hash_lp *table, size_t new_capacity) {
    size_t i;
    struct algokit_hash_lp_entry *old_entries = table->entries;
    size_t old_capacity = table->capacity;

    table->entries = (struct algokit_hash_lp_entry *)calloc(new_capacity, sizeof(*table->entries));
    if (!table->entries) {
        table->entries = old_entries;
        return -1;
    }

    table->capacity = new_capacity;
    table->size = 0;

    for (i = 0; i < old_capacity; i++) {
        if (old_entries[i].state == 1) {
            algokit_hash_lp_put(table, old_entries[i].item);
        }
    }

    free(old_entries);
    return 0;
}

algokit_hash_lp *algokit_hash_lp_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp) {
    algokit_hash_lp *table;

    if (!hash || !cmp || capacity == 0) {
        return NULL;
    }

    table = (algokit_hash_lp *)malloc(sizeof(*table));
    if (!table) {
        return NULL;
    }

    table->entries = (struct algokit_hash_lp_entry *)calloc(capacity, sizeof(*table->entries));
    if (!table->entries) {
        free(table);
        return NULL;
    }

    table->capacity = capacity;
    table->size = 0;
    table->hash = hash;
    table->cmp = cmp;
    return table;
}

void algokit_hash_lp_destroy(algokit_hash_lp *table, algokit_item_destroy_fn destroy) {
    size_t i;

    if (!table) {
        return;
    }

    if (destroy) {
        for (i = 0; i < table->capacity; i++) {
            if (table->entries[i].state == 1) {
                destroy(table->entries[i].item.key, table->entries[i].item.val);
            }
        }
    }

    free(table->entries);
    free(table);
}

int algokit_hash_lp_put(algokit_hash_lp *table, Item item) {
    size_t idx;
    size_t first_tombstone = (size_t)-1;

    if (!table) {
        return -1;
    }

    if ((table->size + 1) * 10 >= table->capacity * 7) {
        if (algokit_hash_lp_resize(table, table->capacity * 2) != 0) {
            return -1;
        }
    }

    idx = algokit_hash_lp_index(table, item.key);
    while (table->entries[idx].state != 0) {
        if (table->entries[idx].state == 1 && table->cmp(table->entries[idx].item.key, item.key) == 0) {
            return 1;
        }
        if (table->entries[idx].state == 2 && first_tombstone == (size_t)-1) {
            first_tombstone = idx;
        }
        idx = (idx + 1) % table->capacity;
    }

    if (first_tombstone != (size_t)-1) {
        idx = first_tombstone;
    }

    table->entries[idx].item = item;
    table->entries[idx].state = 1;
    table->size++;
    return 0;
}

int algokit_hash_lp_get(const algokit_hash_lp *table, const void *key, Item *out) {
    size_t idx;

    if (!table || !key || !out) {
        return -1;
    }

    idx = algokit_hash_lp_index(table, key);
    while (table->entries[idx].state != 0) {
        if (table->entries[idx].state == 1 && table->cmp(table->entries[idx].item.key, key) == 0) {
            *out = table->entries[idx].item;
            return 0;
        }
        idx = (idx + 1) % table->capacity;
    }

    return 1;
}

int algokit_hash_lp_delete(algokit_hash_lp *table, const void *key, Item *out) {
    size_t idx;

    if (!table || !key || !out) {
        return -1;
    }

    idx = algokit_hash_lp_index(table, key);
    while (table->entries[idx].state != 0) {
        if (table->entries[idx].state == 1 && table->cmp(table->entries[idx].item.key, key) == 0) {
            *out = table->entries[idx].item;
            table->entries[idx].state = 2;
            table->size--;
            if (table->capacity > 8 && table->size * 10 <= table->capacity * 2) {
                algokit_hash_lp_resize(table, table->capacity / 2);
            }
            return 0;
        }
        idx = (idx + 1) % table->capacity;
    }

    return 1;
}
