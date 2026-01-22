#include "algokit/uf.h"

#include <stdlib.h>

struct algokit_uf_qf {
    size_t n;
    size_t *id;
};

algokit_uf_qf *algokit_uf_qf_create(size_t n) {
    size_t i;
    algokit_uf_qf *uf = (algokit_uf_qf *)malloc(sizeof(*uf));

    if (!uf) {
        return NULL;
    }

    uf->id = (size_t *)malloc(n * sizeof(*uf->id));
    if (!uf->id) {
        free(uf);
        return NULL;
    }

    uf->n = n;
    for (i = 0; i < n; i++) {
        uf->id[i] = i;
    }

    return uf;
}

void algokit_uf_qf_destroy(algokit_uf_qf *uf) {
    if (!uf) {
        return;
    }

    free(uf->id);
    free(uf);
}

int algokit_uf_qf_find(const algokit_uf_qf *uf, size_t p, size_t *out_root) {
    if (!uf || !out_root || p >= uf->n) {
        return -1;
    }

    *out_root = uf->id[p];
    return 0;
}

int algokit_uf_qf_union(algokit_uf_qf *uf, size_t p, size_t q) {
    size_t i;
    size_t pid;
    size_t qid;

    if (!uf || p >= uf->n || q >= uf->n) {
        return -1;
    }

    pid = uf->id[p];
    qid = uf->id[q];
    if (pid == qid) {
        return 0;
    }

    for (i = 0; i < uf->n; i++) {
        if (uf->id[i] == pid) {
            uf->id[i] = qid;
        }
    }

    return 0;
}

int algokit_uf_qf_connected(const algokit_uf_qf *uf, size_t p, size_t q, int *out) {
    if (!uf || !out || p >= uf->n || q >= uf->n) {
        return -1;
    }

    *out = (uf->id[p] == uf->id[q]) ? 1 : 0;
    return 0;
}
