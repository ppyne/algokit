#include <stdio.h>
#include <stddef.h>

#include "algokit/graph.h"

/*
 * Context: compute MSTs on a weighted network using Prim and Kruskal.
 * We print parent links by key for each algorithm.
 */

static size_t int_hash(const void *key) {
    return (size_t)(*(const int *)key * 2654435761u);
}

static int int_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void print_parent(const algokit_graph *graph, const char *label, const size_t *parent) {
    size_t i;
    printf("%s:\n", label);
    for (i = 0; i < algokit_graph_vertex_count(graph); i++) {
        const int *key = (const int *)algokit_graph_vertex_key(graph, i);
        if (parent[i] == (size_t)-1) {
            printf("root: %d\n", *key);
        } else {
            const int *pkey = (const int *)algokit_graph_vertex_key(graph, parent[i]);
            printf("%d <- %d\n", *key, *pkey);
        }
    }
}

int main(void) {
    algokit_graph *graph = algokit_graph_create(8, int_hash, int_cmp);
    int v0 = 0, v1 = 1, v2 = 2, v3 = 3;
    size_t parent[4];

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
    algokit_graph_add_edge(graph, &v1, &v2, 1.0);
    algokit_graph_add_edge(graph, &v0, &v2, 2.5);
    algokit_graph_add_edge(graph, &v2, &v3, 0.5);

    /* Compute Prim MST from vertex 0. */
    algokit_graph_mst_prim(graph, &v0, parent);
    print_parent(graph, "prim", parent);

    /* Compute Kruskal MST. */
    algokit_graph_mst_kruskal(graph, parent);
    print_parent(graph, "kruskal", parent);

    /* Destroy the graph. */
    algokit_graph_destroy(graph, NULL);
    return 0;
}
