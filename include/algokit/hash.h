#ifndef ALGOKIT_HASH_H
#define ALGOKIT_HASH_H

#include <stddef.h>
#include "algokit/item.h"

/*
 * Hash tables with two implementations:
 * - Linear probing
 * - Separate chaining
 *
 * Keys are hashed by the user-provided hash function and compared by cmp.
 */

typedef struct algokit_hash_lp algokit_hash_lp;
typedef struct algokit_hash_sc algokit_hash_sc;

/* Linear probing */
algokit_hash_lp *algokit_hash_lp_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp);
void algokit_hash_lp_destroy(algokit_hash_lp *table, algokit_item_destroy_fn destroy);
int algokit_hash_lp_put(algokit_hash_lp *table, Item item);
int algokit_hash_lp_get(const algokit_hash_lp *table, const void *key, Item *out);
int algokit_hash_lp_delete(algokit_hash_lp *table, const void *key, Item *out);

/* Separate chaining */
algokit_hash_sc *algokit_hash_sc_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp);
void algokit_hash_sc_destroy(algokit_hash_sc *table, algokit_item_destroy_fn destroy);
int algokit_hash_sc_put(algokit_hash_sc *table, Item item);
int algokit_hash_sc_get(const algokit_hash_sc *table, const void *key, Item *out);
int algokit_hash_sc_delete(algokit_hash_sc *table, const void *key, Item *out);

#endif /* ALGOKIT_HASH_H */
