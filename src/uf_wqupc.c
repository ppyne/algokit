#include "algokit/uf.h"

#include <stdlib.h>

struct algokit_uf_wqupc {
    size_t n;
    size_t *parent;
    size_t *size;
};

static size_t algokit_uf_wqupc_root(algokit_uf_wqupc *uf, size_t p) {
    while (p != uf->parent[p]) {
        uf->parent[p] = uf->parent[uf->parent[p]];
        p = uf->parent[p];
    }
    return p;
}

algokit_uf_wqupc *algokit_uf_wqupc_create(size_t n) {
    size_t i;
    algokit_uf_wqupc *uf = (algokit_uf_wqupc *)malloc(sizeof(*uf));

    if (!uf) {
        return NULL;
    }

    uf->parent = (size_t *)malloc(n * sizeof(*uf->parent));
    uf->size = (size_t *)malloc(n * sizeof(*uf->size));
    if (!uf->parent || !uf->size) {
        free(uf->parent);
        free(uf->size);
        free(uf);
        return NULL;
    }

    uf->n = n;
    for (i = 0; i < n; i++) {
        uf->parent[i] = i;
        uf->size[i] = 1;
    }

    return uf;
}

void algokit_uf_wqupc_destroy(algokit_uf_wqupc *uf) {
    if (!uf) {
        return;
    }

    free(uf->parent);
    free(uf->size);
    free(uf);
}

int algokit_uf_wqupc_find(algokit_uf_wqupc *uf, size_t p, size_t *out_root) {
    if (!uf || !out_root || p >= uf->n) {
        return -1;
    }

    *out_root = algokit_uf_wqupc_root(uf, p);
    return 0;
}

int algokit_uf_wqupc_union(algokit_uf_wqupc *uf, size_t p, size_t q) {
    size_t root_p;
    size_t root_q;

    if (!uf || p >= uf->n || q >= uf->n) {
        return -1;
    }

    root_p = algokit_uf_wqupc_root(uf, p);
    root_q = algokit_uf_wqupc_root(uf, q);

    if (root_p == root_q) {
        return 0;
    }

    if (uf->size[root_p] < uf->size[root_q]) {
        uf->parent[root_p] = root_q;
        uf->size[root_q] += uf->size[root_p];
    } else {
        uf->parent[root_q] = root_p;
        uf->size[root_p] += uf->size[root_q];
    }

    return 0;
}

int algokit_uf_wqupc_connected(algokit_uf_wqupc *uf, size_t p, size_t q, int *out) {
    if (!uf || !out || p >= uf->n || q >= uf->n) {
        return -1;
    }

    *out = (algokit_uf_wqupc_root(uf, p) == algokit_uf_wqupc_root(uf, q)) ? 1 : 0;
    return 0;
}
