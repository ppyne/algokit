#include <stdio.h>

#include "algokit/uf.h"

/*
 * Context: merge connections over time while keeping operations fast.
 * We use the weighted + path compression variant for efficiency.
 */
int main(void) {
    int connected;
    size_t root;
    algokit_uf_wqupc *uf = algokit_uf_wqupc_create(8);

    /* Create the structure for 8 items. */
    if (!uf) {
        return 1;
    }

    /* Merge some pairs to form clusters. */
    if (algokit_uf_wqupc_union(uf, 0, 1) != 0 ||
        algokit_uf_wqupc_union(uf, 1, 2) != 0 ||
        algokit_uf_wqupc_union(uf, 3, 4) != 0 ||
        algokit_uf_wqupc_union(uf, 4, 5) != 0) {
        algokit_uf_wqupc_destroy(uf);
        return 1;
    }

    /* Check connectivity after unions. */
    if (algokit_uf_wqupc_connected(uf, 0, 2, &connected) != 0) {
        algokit_uf_wqupc_destroy(uf);
        return 1;
    }
    printf("0 and 2 connected: %d\n", connected);

    /* Find a representative and compress paths. */
    if (algokit_uf_wqupc_find(uf, 5, &root) != 0) {
        algokit_uf_wqupc_destroy(uf);
        return 1;
    }
    printf("root of 5: %zu\n", root);

    /* Clean up internal memory. */
    algokit_uf_wqupc_destroy(uf);
    return 0;
}
