#include "algokit/uf.h"

#include <stdlib.h>

struct algokit_uf_qu {
    size_t n;
    size_t *parent;
};

static size_t algokit_uf_qu_root(const algokit_uf_qu *uf, size_t p) {
    while (p != uf->parent[p]) {
        p = uf->parent[p];
    }
    return p;
}

algokit_uf_qu *algokit_uf_qu_create(size_t n) {
    size_t i;
    algokit_uf_qu *uf = (algokit_uf_qu *)malloc(sizeof(*uf));

    if (!uf) {
        return NULL;
    }

    uf->parent = (size_t *)malloc(n * sizeof(*uf->parent));
    if (!uf->parent) {
        free(uf);
        return NULL;
    }

    uf->n = n;
    for (i = 0; i < n; i++) {
        uf->parent[i] = i;
    }

    return uf;
}

void algokit_uf_qu_destroy(algokit_uf_qu *uf) {
    if (!uf) {
        return;
    }

    free(uf->parent);
    free(uf);
}

int algokit_uf_qu_find(const algokit_uf_qu *uf, size_t p, size_t *out_root) {
    if (!uf || !out_root || p >= uf->n) {
        return -1;
    }

    *out_root = algokit_uf_qu_root(uf, p);
    return 0;
}

int algokit_uf_qu_union(algokit_uf_qu *uf, size_t p, size_t q) {
    size_t root_p;
    size_t root_q;

    if (!uf || p >= uf->n || q >= uf->n) {
        return -1;
    }

    root_p = algokit_uf_qu_root(uf, p);
    root_q = algokit_uf_qu_root(uf, q);

    if (root_p == root_q) {
        return 0;
    }

    uf->parent[root_p] = root_q;
    return 0;
}

int algokit_uf_qu_connected(const algokit_uf_qu *uf, size_t p, size_t q, int *out) {
    if (!uf || !out || p >= uf->n || q >= uf->n) {
        return -1;
    }

    *out = (algokit_uf_qu_root(uf, p) == algokit_uf_qu_root(uf, q)) ? 1 : 0;
    return 0;
}
