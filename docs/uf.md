# Union-Find (Disjoint Set Union)

Union-Find keeps track of which elements belong to the same group (connected component). It supports two core actions: merging two groups and checking whether two elements are connected.

## When to use it

- Connected components in graphs
- Dynamic connectivity (edges added over time)
- Grouping users, devices, or IDs into clusters

## What algokit provides

algokit provides three Union-Find implementations. All use zero-based indices in the range `[0, n)`.

- Quick-Find (fast `find`, slow `union`)
- Quick-Union (tree-based)
- Weighted Quick-Union with path compression (fastest in practice)

### Common concepts

- Each element is identified by an index `p` or `q` (`size_t`).
- `n` is the number of elements.
- Functions return `0` on success, nonzero on failure.
- Outputs are returned via pointer parameters.

### Quick-Find API

- `algokit_uf_qf_create(size_t n)`
  - Creates a Quick-Find structure for `n` elements.
  - Returns: `algokit_uf_qf *` or `NULL` on failure.

- `algokit_uf_qf_destroy(algokit_uf_qf *uf)`
  - Frees a Quick-Find structure. `uf` may be `NULL`.

- `algokit_uf_qf_find(const algokit_uf_qf *uf, size_t p, size_t *out_root)`
  - Reads the component id for `p` in Quick-Find.
  - Parameters: `p` must be in `[0, n)`, `out_root` must not be `NULL`.

- `algokit_uf_qf_union(algokit_uf_qf *uf, size_t p, size_t q)`
  - Merges two Quick-Find components.
  - Parameters: `p`, `q` must be valid indices.

- `algokit_uf_qf_connected(const algokit_uf_qf *uf, size_t p, size_t q, int *out)`
  - Checks whether two Quick-Find elements are connected.
  - Writes `1` to `*out` if `p` and `q` are connected, else `0`.

### Quick-Union API

- `algokit_uf_qu_create(size_t n)`
  - Creates a Quick-Union structure for `n` elements.
  - Returns: `algokit_uf_qu *` or `NULL` on failure.

- `algokit_uf_qu_destroy(algokit_uf_qu *uf)`
  - Frees a Quick-Union structure. `uf` may be `NULL`.

- `algokit_uf_qu_find(const algokit_uf_qu *uf, size_t p, size_t *out_root)`
  - Finds the root representative of `p` in Quick-Union.
  - Parameters: `p` must be in `[0, n)`, `out_root` must not be `NULL`.

- `algokit_uf_qu_union(algokit_uf_qu *uf, size_t p, size_t q)`
  - Connects two Quick-Union components by linking roots.
  - Parameters: `p`, `q` must be valid indices.

- `algokit_uf_qu_connected(const algokit_uf_qu *uf, size_t p, size_t q, int *out)`
  - Checks whether two Quick-Union elements are connected.
  - Writes `1` to `*out` if `p` and `q` are connected, else `0`.
  - Parameters: `out` must not be `NULL`.

These functions mirror Quick-Find but use parent pointers to represent trees.

### Weighted Quick-Union with path compression API

- `algokit_uf_wqupc_create(size_t n)`
  - Creates a weighted Quick-Union structure with path compression.
  - Returns: `algokit_uf_wqupc *` or `NULL` on failure.

- `algokit_uf_wqupc_destroy(algokit_uf_wqupc *uf)`
  - Frees a weighted Quick-Union structure with path compression. `uf` may be `NULL`.

- `algokit_uf_wqupc_find(algokit_uf_wqupc *uf, size_t p, size_t *out_root)`
  - Finds the root representative of `p` with path compression.
  - Performs path compression while searching.
  - Parameters: `p` must be in `[0, n)`, `out_root` must not be `NULL`.

- `algokit_uf_wqupc_union(algokit_uf_wqupc *uf, size_t p, size_t q)`
  - Connects two components by linking smaller trees under larger ones.
  - Parameters: `p`, `q` must be valid indices.

- `algokit_uf_wqupc_connected(algokit_uf_wqupc *uf, size_t p, size_t q, int *out)`
  - Checks whether two weighted Quick-Union elements are connected.
  - Writes `1` to `*out` if `p` and `q` are connected, else `0`.
  - Parameters: `out` must not be `NULL`.

These functions also mirror Quick-Find but maintain tree sizes and compress paths during `find`.

### Limits and constraints

- Indices outside `[0, n)` are invalid.
- All implementations store internal arrays of size `n`.

## Key points

- Quick-Find is simple but slow to union.
  - Illustration: `algokit_uf_qf_union` scans the entire array to relabel a component.
- Quick-Union is faster to union but can create tall trees.
  - Illustration: repeated unions without weighting can degrade performance.
- Weighted Quick-Union with path compression is fastest in practice.
  - Illustration: `algokit_uf_wqupc_find` flattens paths during access.

## Example: Quick-Find (connected components)

Context: We model friendships between users. Each user has an id from 0 to 9.
We union pairs of users who are connected, then check whether two users are in the same group.

```c
#include <stdio.h>
#include "algokit/uf.h"

int main(void) {
    size_t i;
    int connected;
    size_t root;
    size_t edges[][2] = {
        {0, 1}, {2, 3}, {4, 5}, {6, 7}, {7, 8}
    };
    algokit_uf_qf *uf = algokit_uf_qf_create(10);

    /* Create the structure for 10 users. */
    if (!uf) {
        return 1;
    }

    /* Merge users according to the edge list. */
    for (i = 0; i < sizeof(edges) / sizeof(edges[0]); i++) {
        algokit_uf_qf_union(uf, edges[i][0], edges[i][1]);
    }

    /* Query connectivity between two users. */
    algokit_uf_qf_connected(uf, 0, 1, &connected);
    printf("0 and 1 connected: %d\n", connected);

    /* Find the component id for a user. */
    algokit_uf_qf_find(uf, 7, &root);
    printf("root of 7: %zu\n", root);

    /* Clean up internal memory. */
    algokit_uf_qf_destroy(uf);
    return 0;
}
```

## Example: Quick-Union (connected components)

Context: We group device IDs that share the same network segment. We union pairs and test connectivity.

```c
#include <stdio.h>
#include "algokit/uf.h"

int main(void) {
    int connected;
    size_t root;
    algokit_uf_qu *uf = algokit_uf_qu_create(6);

    /* Create the structure for 6 devices. */
    if (!uf) {
        return 1;
    }

    /* Build two groups: {0,1,2} and {3,4}. */
    algokit_uf_qu_union(uf, 0, 1);
    algokit_uf_qu_union(uf, 1, 2);
    algokit_uf_qu_union(uf, 3, 4);

    /* Check connectivity between a pair of devices. */
    algokit_uf_qu_connected(uf, 0, 2, &connected);
    printf("0 and 2 connected: %d\n", connected);

    /* Find the root of a device. */
    algokit_uf_qu_find(uf, 4, &root);
    printf("root of 4: %zu\n", root);

    /* Clean up internal memory. */
    algokit_uf_qu_destroy(uf);
    return 0;
}
```

## Example: Weighted Quick-Union with path compression

Context: We track which items are in the same cluster as connections arrive over time.
We use the optimized variant to keep operations fast as the structure grows.

```c
#include <stdio.h>
#include "algokit/uf.h"

int main(void) {
    int connected;
    size_t root;
    algokit_uf_wqupc *uf = algokit_uf_wqupc_create(8);

    /* Create the structure for 8 items. */
    if (!uf) {
        return 1;
    }

    /* Merge some pairs to form clusters. */
    algokit_uf_wqupc_union(uf, 0, 1);
    algokit_uf_wqupc_union(uf, 1, 2);
    algokit_uf_wqupc_union(uf, 3, 4);
    algokit_uf_wqupc_union(uf, 4, 5);

    /* Check connectivity after unions. */
    algokit_uf_wqupc_connected(uf, 0, 2, &connected);
    printf("0 and 2 connected: %d\n", connected);

    /* Find a representative and compress paths. */
    algokit_uf_wqupc_find(uf, 5, &root);
    printf("root of 5: %zu\n", root);

    /* Clean up internal memory. */
    algokit_uf_wqupc_destroy(uf);
    return 0;
}
```
