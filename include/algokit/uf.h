#ifndef ALGOKIT_UF_H
#define ALGOKIT_UF_H

#include <stddef.h>

/*
 * Union-Find (Disjoint Set Union)
 * Three implementations are provided: Quick-Find, Quick-Union, and Weighted
 * Quick-Union with path compression.
 *
 * API conventions:
 * - Indices are in the range [0, n).
 * - Functions returning int use 0 for success, nonzero for failure.
 * - Output values are returned via pointer parameters.
 */

typedef struct algokit_uf_qf algokit_uf_qf;
typedef struct algokit_uf_qu algokit_uf_qu;
typedef struct algokit_uf_wqupc algokit_uf_wqupc;

/* Quick-Find */
algokit_uf_qf *algokit_uf_qf_create(size_t n);
void algokit_uf_qf_destroy(algokit_uf_qf *uf);
int algokit_uf_qf_find(const algokit_uf_qf *uf, size_t p, size_t *out_root);
int algokit_uf_qf_union(algokit_uf_qf *uf, size_t p, size_t q);
int algokit_uf_qf_connected(const algokit_uf_qf *uf, size_t p, size_t q, int *out);

/* Quick-Union */
algokit_uf_qu *algokit_uf_qu_create(size_t n);
void algokit_uf_qu_destroy(algokit_uf_qu *uf);
int algokit_uf_qu_find(const algokit_uf_qu *uf, size_t p, size_t *out_root);
int algokit_uf_qu_union(algokit_uf_qu *uf, size_t p, size_t q);
int algokit_uf_qu_connected(const algokit_uf_qu *uf, size_t p, size_t q, int *out);

/* Weighted Quick-Union with path compression */
algokit_uf_wqupc *algokit_uf_wqupc_create(size_t n);
void algokit_uf_wqupc_destroy(algokit_uf_wqupc *uf);
int algokit_uf_wqupc_find(algokit_uf_wqupc *uf, size_t p, size_t *out_root);
int algokit_uf_wqupc_union(algokit_uf_wqupc *uf, size_t p, size_t q);
int algokit_uf_wqupc_connected(algokit_uf_wqupc *uf, size_t p, size_t q, int *out);

#endif /* ALGOKIT_UF_H */
