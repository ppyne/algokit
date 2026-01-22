#include <stdio.h>

#include "algokit/graph.h"

/*
 * Context: model a small network of cities with weighted connections.
 * We run BFS/DFS and compute connected components.
 */

static size_t int_hash(const void *key) {
    return (size_t)(*(const int *)key * 2654435761u);
}

static int int_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void print_key(const void *key, void *ctx) {
    (void)ctx;
    printf("%d\n", *(const int *)key);
}

int main(void) {
    algokit_graph *graph = algokit_graph_create(8, int_hash, int_cmp);
    int v0 = 0, v1 = 1, v2 = 2, v3 = 3;
    size_t component_ids[4];
    size_t i;

    /* Create the graph before adding vertices. */
    if (!graph) {
        return 1;
    }

    /* Add vertices with user-defined keys. */
    algokit_graph_add_vertex(graph, &v0);
    algokit_graph_add_vertex(graph, &v1);
    algokit_graph_add_vertex(graph, &v2);
    algokit_graph_add_vertex(graph, &v3);

    /* Add undirected weighted edges. */
    algokit_graph_add_edge(graph, &v0, &v1, 2.0);
    algokit_graph_add_edge(graph, &v1, &v2, 1.5);

    /* Run BFS from vertex 0. */
    printf("bfs:\n");
    algokit_graph_bfs(graph, &v0, print_key, NULL);

    /* Run DFS from vertex 0. */
    printf("dfs:\n");
    algokit_graph_dfs(graph, &v0, print_key, NULL);

    /* Compute connected components. */
    algokit_graph_connected_components(graph, component_ids);
    for (i = 0; i < algokit_graph_vertex_count(graph); i++) {
        const int *key = (const int *)algokit_graph_vertex_key(graph, i);
        printf("component: %d -> %zu\n", *key, component_ids[i]);
    }

    /* Destroy the graph; keys are stack-allocated. */
    algokit_graph_destroy(graph, NULL);
    return 0;
}
