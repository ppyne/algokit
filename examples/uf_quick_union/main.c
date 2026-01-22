#include <stdio.h>

#include "algokit/uf.h"

/*
 * Context: group device ids by network segment and check connectivity.
 * We merge known links and then check whether two devices share a segment.
 */
int main(void) {
    int connected;
    size_t root;
    algokit_uf_qu *uf = algokit_uf_qu_create(6);

    /* Create the structure for 6 devices. */
    if (!uf) {
        return 1;
    }

    /* Build two groups: {0,1,2} and {3,4}. */
    if (algokit_uf_qu_union(uf, 0, 1) != 0 ||
        algokit_uf_qu_union(uf, 1, 2) != 0 ||
        algokit_uf_qu_union(uf, 3, 4) != 0) {
        algokit_uf_qu_destroy(uf);
        return 1;
    }

    /* Check connectivity between a pair of devices. */
    if (algokit_uf_qu_connected(uf, 0, 2, &connected) != 0) {
        algokit_uf_qu_destroy(uf);
        return 1;
    }
    printf("0 and 2 connected: %d\n", connected);

    /* Find the root of a device. */
    if (algokit_uf_qu_find(uf, 4, &root) != 0) {
        algokit_uf_qu_destroy(uf);
        return 1;
    }
    printf("root of 4: %zu\n", root);

    /* Clean up internal memory. */
    algokit_uf_qu_destroy(uf);
    return 0;
}
