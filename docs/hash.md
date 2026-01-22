# Hash tables

This module provides two hash table implementations: linear probing and separate chaining. Both store `Item` values and use user-provided hash and comparison functions.

## When to use it

- Fast key/value lookups
- Dictionaries and caches
- Large datasets with unpredictable key order

## What algokit provides

Both hash tables share the same high-level operations:

- `put`: insert a new key/value pair
- `get`: retrieve an existing key/value pair
- `delete`: remove an existing key/value pair

### Common concepts

- `hash`: function that maps a key to a `size_t` hash value.
- `cmp`: function that compares keys and returns <0, 0, >0.
- Keys must remain valid while stored in the table.
- Duplicate keys are rejected (put returns `1`).

### Linear probing API

- `algokit_hash_lp_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp)`
  - Creates a linear probing table with the given capacity.
  - Parameters: `capacity` must be > 0, `hash` and `cmp` must not be `NULL`.
  - Returns: `algokit_hash_lp *` or `NULL` on failure.

- `algokit_hash_lp_destroy(algokit_hash_lp *table, algokit_item_destroy_fn destroy)`
  - Frees the table and its internal array.
  - Parameters: `table` may be `NULL`, `destroy` frees `key`/`val` if provided.

- `algokit_hash_lp_put(algokit_hash_lp *table, Item item)`
  - Inserts a new item if the key does not exist.
  - Parameters: `table` must not be `NULL`.
  - Returns: `0` on success, `1` if key exists, nonzero on error.

- `algokit_hash_lp_get(const algokit_hash_lp *table, const void *key, Item *out)`
  - Retrieves the item matching `key` into `*out`.
  - Parameters: `table`, `key`, `out` must not be `NULL`.
  - Returns: `0` on success, `1` if not found, nonzero on error.

- `algokit_hash_lp_delete(algokit_hash_lp *table, const void *key, Item *out)`
  - Removes the item matching `key` and writes it to `*out`.
  - Parameters: `table`, `key`, `out` must not be `NULL`.
  - Returns: `0` on success, `1` if not found, nonzero on error.

### Separate chaining API

- `algokit_hash_sc_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp)`
  - Creates a separate chaining table with the given capacity.
  - Parameters: `capacity` must be > 0, `hash` and `cmp` must not be `NULL`.
  - Returns: `algokit_hash_sc *` or `NULL` on failure.

- `algokit_hash_sc_destroy(algokit_hash_sc *table, algokit_item_destroy_fn destroy)`
  - Frees the table and its bucket lists.
  - Parameters: `table` may be `NULL`, `destroy` frees `key`/`val` if provided.

- `algokit_hash_sc_put(algokit_hash_sc *table, Item item)`
  - Inserts a new item if the key does not exist.
  - Parameters: `table` must not be `NULL`.
  - Returns: `0` on success, `1` if key exists, nonzero on error.

- `algokit_hash_sc_get(const algokit_hash_sc *table, const void *key, Item *out)`
  - Retrieves the item matching `key` into `*out`.
  - Parameters: `table`, `key`, `out` must not be `NULL`.
  - Returns: `0` on success, `1` if not found, nonzero on error.

- `algokit_hash_sc_delete(algokit_hash_sc *table, const void *key, Item *out)`
  - Removes the item matching `key` and writes it to `*out`.
  - Parameters: `table`, `key`, `out` must not be `NULL`.
  - Returns: `0` on success, `1` if not found, nonzero on error.

### Limits and constraints

- Linear probing uses tombstones; performance depends on load factor.
- Separate chaining uses linked lists; performance depends on bucket distribution.

## Key points

- The hash function determines distribution quality.
  - Illustration: poor hashing increases collisions and slows lookups.
- Linear probing is cache-friendly but sensitive to load factor.
  - Illustration: resizing keeps the load factor below ~0.7.
- Separate chaining tolerates higher load factors but allocates nodes.
  - Illustration: linked lists grow as collisions occur.

## Examples

Each implementation has a dedicated example directory under `examples/` with a local `Makefile`:

- `examples/hash_linear_probing/`
- `examples/hash_separate_chaining/`

Every example includes a context introduction and commented code steps. The listings below mirror the example sources.

### Example: linear probing

Context: store user emails keyed by integer IDs. We insert, retrieve, and delete entries while keeping ownership clear.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
```

### Example: separate chaining

Context: store inventory counts keyed by integer product IDs. We insert, retrieve, and delete entries using chains.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
```
