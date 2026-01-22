#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "algokit/uf.h"

int main(void) {
    int connected;
    size_t root;
    algokit_uf_wqupc *uf = algokit_uf_wqupc_create(8);

    printf("uf_wqupc_test: create\n");
    assert(uf != NULL);

    printf("uf_wqupc_test: union\n");
    assert(algokit_uf_wqupc_union(uf, 0, 1) == 0);
    assert(algokit_uf_wqupc_union(uf, 1, 2) == 0);
    assert(algokit_uf_wqupc_union(uf, 3, 4) == 0);
    assert(algokit_uf_wqupc_union(uf, 4, 5) == 0);

    printf("uf_wqupc_test: connected\n");
    assert(algokit_uf_wqupc_connected(uf, 0, 2, &connected) == 0);
    assert(connected == 1);

    assert(algokit_uf_wqupc_connected(uf, 0, 5, &connected) == 0);
    assert(connected == 0);

    printf("uf_wqupc_test: find\n");
    assert(algokit_uf_wqupc_find(uf, 5, &root) == 0);
    assert(root == 3 || root == 4 || root == 5);

    printf("uf_wqupc_test: destroy\n");
    algokit_uf_wqupc_destroy(uf);
    printf("uf_wqupc_test: ok\n");
    return 0;
}
