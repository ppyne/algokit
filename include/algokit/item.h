#ifndef ALGOKIT_ITEM_H
#define ALGOKIT_ITEM_H

#include <stddef.h>

/*
 * Global conventions
 * - Naming: public symbols use the algokit_ prefix.
 * - Errors: functions returning int use 0 for success, nonzero for failure;
 *   functions returning pointers return NULL on error unless documented otherwise.
 * - Ownership: the library never allocates or frees user data implicitly.
 *   The caller owns key/val memory and controls allocation and deallocation.
 */

typedef struct {
    void *key;
    void *val;
} Item;

/*
 * User-provided callbacks
 * - Comparison returns <0, 0, >0 for less/equal/greater keys.
 * - Hash returns a stable hash for a key.
 * - Destroy releases key/val ownership when the user requests it.
 */

typedef int (*algokit_key_cmp_fn)(const void *a, const void *b);

typedef size_t (*algokit_key_hash_fn)(const void *key);

typedef void (*algokit_item_destroy_fn)(void *key, void *val);

#endif /* ALGOKIT_ITEM_H */
