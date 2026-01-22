#include <stdio.h>
#include <stddef.h>

#include "algokit/digraph.h"

/*
 * Context: model build dependencies and compute a topological order.
 * We also check for cycles before using the order.
 */

static size_t int_hash(const void *key) {
    return (size_t)(*(const int *)key * 2654435761u);
}

static int int_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void print_order(const algokit_digraph *graph, const size_t *order) {
    size_t i;
    for (i = 0; i < algokit_digraph_vertex_count(graph); i++) {
        const int *key = (const int *)algokit_digraph_vertex_key(graph, order[i]);
        printf("%d\n", *key);
    }
}

int main(void) {
    algokit_digraph *graph = algokit_digraph_create(8, int_hash, int_cmp);
    int v0 = 0, v1 = 1, v2 = 2, v3 = 3;
    size_t order[4];
    int has_cycle;

    /* Create the graph before adding vertices. */
    if (!graph) {
        return 1;
    }

    /* Add vertices with user-defined keys. */
    algokit_digraph_add_vertex(graph, &v0);
    algokit_digraph_add_vertex(graph, &v1);
    algokit_digraph_add_vertex(graph, &v2);
    algokit_digraph_add_vertex(graph, &v3);

    /* Add directed edges (dependencies). */
    algokit_digraph_add_edge(graph, &v0, &v1);
    algokit_digraph_add_edge(graph, &v1, &v2);
    algokit_digraph_add_edge(graph, &v0, &v3);

    /* Detect cycles. */
    algokit_digraph_has_cycle(graph, &has_cycle);
    printf("cycle: %d\n", has_cycle);

    /* Compute topological order. */
    if (algokit_digraph_topological_sort(graph, order) == 0) {
        printf("order:\n");
        print_order(graph, order);
    }

    /* Destroy the graph. */
    algokit_digraph_destroy(graph, NULL);
    return 0;
}
