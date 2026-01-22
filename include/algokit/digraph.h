#ifndef ALGOKIT_DIGRAPH_H
#define ALGOKIT_DIGRAPH_H

#include <stddef.h>
#include "algokit/item.h"

/*
 * Directed graph (unweighted) using adjacency lists.
 * Vertex identifiers are user-defined keys.
 */

typedef struct algokit_digraph algokit_digraph;

typedef void (*algokit_digraph_visit_fn)(const void *key, void *ctx);

algokit_digraph *algokit_digraph_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp);

void algokit_digraph_destroy(algokit_digraph *graph, algokit_item_destroy_fn destroy);

int algokit_digraph_add_vertex(algokit_digraph *graph, void *key);

int algokit_digraph_add_edge(algokit_digraph *graph, const void *from_key, const void *to_key);

size_t algokit_digraph_vertex_count(const algokit_digraph *graph);

const void *algokit_digraph_vertex_key(const algokit_digraph *graph, size_t index);

int algokit_digraph_bfs(const algokit_digraph *graph, const void *start_key, algokit_digraph_visit_fn visit, void *ctx);

int algokit_digraph_dfs(const algokit_digraph *graph, const void *start_key, algokit_digraph_visit_fn visit, void *ctx);

int algokit_digraph_topological_sort(const algokit_digraph *graph, size_t *order);

int algokit_digraph_has_cycle(const algokit_digraph *graph, int *out);

#endif /* ALGOKIT_DIGRAPH_H */
