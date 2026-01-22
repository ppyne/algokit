#include <stdio.h>

#include "algokit/digraph.h"

/*
 * Context: model a directed workflow and traverse reachable nodes.
 * We run BFS and DFS from the start node.
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
    algokit_digraph *graph = algokit_digraph_create(8, int_hash, int_cmp);
    int v0 = 0, v1 = 1, v2 = 2, v3 = 3;

    /* Create the graph before adding vertices. */
    if (!graph) {
        return 1;
    }

    /* Add vertices with user-defined keys. */
    algokit_digraph_add_vertex(graph, &v0);
    algokit_digraph_add_vertex(graph, &v1);
    algokit_digraph_add_vertex(graph, &v2);
    algokit_digraph_add_vertex(graph, &v3);

    /* Add directed edges (workflow). */
    algokit_digraph_add_edge(graph, &v0, &v1);
    algokit_digraph_add_edge(graph, &v1, &v2);
    algokit_digraph_add_edge(graph, &v0, &v3);

    /* Run BFS from vertex 0. */
    printf("bfs:\n");
    algokit_digraph_bfs(graph, &v0, print_key, NULL);

    /* Run DFS from vertex 0. */
    printf("dfs:\n");
    algokit_digraph_dfs(graph, &v0, print_key, NULL);

    /* Destroy the graph; keys are stack-allocated. */
    algokit_digraph_destroy(graph, NULL);
    return 0;
}
