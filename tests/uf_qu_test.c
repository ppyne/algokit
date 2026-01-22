#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "algokit/uf.h"

int main(void) {
    int connected;
    size_t root;
    algokit_uf_qu *uf = algokit_uf_qu_create(8);

    printf("uf_qu_test: create\n");
    assert(uf != NULL);

    printf("uf_qu_test: union\n");
    assert(algokit_uf_qu_union(uf, 0, 1) == 0);
    assert(algokit_uf_qu_union(uf, 1, 2) == 0);
    assert(algokit_uf_qu_union(uf, 3, 4) == 0);

    printf("uf_qu_test: connected\n");
    assert(algokit_uf_qu_connected(uf, 0, 2, &connected) == 0);
    assert(connected == 1);

    assert(algokit_uf_qu_connected(uf, 0, 4, &connected) == 0);
    assert(connected == 0);

    printf("uf_qu_test: find\n");
    assert(algokit_uf_qu_find(uf, 2, &root) == 0);
    assert(root == 0 || root == 1 || root == 2);

    printf("uf_qu_test: destroy\n");
    algokit_uf_qu_destroy(uf);
    printf("uf_qu_test: ok\n");
    return 0;
}
