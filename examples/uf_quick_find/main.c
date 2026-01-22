#include <stdio.h>

#include "algokit/uf.h"

/*
 * Context: group users into connected components as friendships are discovered.
 * We store user ids as integers and want to check if two users are connected.
 */
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

    /* Merge users according to the discovered edges. */
    for (i = 0; i < sizeof(edges) / sizeof(edges[0]); i++) {
        if (algokit_uf_qf_union(uf, edges[i][0], edges[i][1]) != 0) {
            algokit_uf_qf_destroy(uf);
            return 1;
        }
    }

    /* Check whether two users are connected. */
    if (algokit_uf_qf_connected(uf, 0, 1, &connected) != 0) {
        algokit_uf_qf_destroy(uf);
        return 1;
    }
    printf("0 and 1 connected: %d\n", connected);

    /* Find the component id for a user. */
    if (algokit_uf_qf_find(uf, 7, &root) != 0) {
        algokit_uf_qf_destroy(uf);
        return 1;
    }
    printf("root of 7: %zu\n", root);

    /* Clean up internal memory. */
    algokit_uf_qf_destroy(uf);
    return 0;
}
