#ifndef ALGOKIT_GRAPH_H
#define ALGOKIT_GRAPH_H

#include <stddef.h>
#include "algokit/item.h"

/*
 * Undirected weighted graph using adjacency lists.
 * Vertex identifiers are user-defined keys.
 */

typedef struct algokit_graph algokit_graph;

typedef void (*algokit_graph_visit_fn)(const void *key, void *ctx);

algokit_graph *algokit_graph_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp);

void algokit_graph_destroy(algokit_graph *graph, algokit_item_destroy_fn destroy);

int algokit_graph_add_vertex(algokit_graph *graph, void *key);

int algokit_graph_add_edge(algokit_graph *graph, const void *u_key, const void *v_key, double weight);

size_t algokit_graph_vertex_count(const algokit_graph *graph);

const void *algokit_graph_vertex_key(const algokit_graph *graph, size_t index);

int algokit_graph_bfs(const algokit_graph *graph, const void *start_key, algokit_graph_visit_fn visit, void *ctx);

int algokit_graph_dfs(const algokit_graph *graph, const void *start_key, algokit_graph_visit_fn visit, void *ctx);

size_t algokit_graph_connected_components(const algokit_graph *graph, size_t *component_ids);

int algokit_graph_mst_prim(const algokit_graph *graph, const void *start_key, size_t *parent);

int algokit_graph_mst_kruskal(const algokit_graph *graph, size_t *parent);

#endif /* ALGOKIT_GRAPH_H */
