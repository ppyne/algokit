#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "algokit/graph.h"

static size_t int_hash(const void *key) {
    return (size_t)(*(const int *)key * 2654435761u);
}

static int int_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

struct visit_ctx {
    size_t count;
};

static void count_visit(const void *key, void *ctx) {
    (void)key;
    ((struct visit_ctx *)ctx)->count++;
}

int main(void) {
    algokit_graph *graph = algokit_graph_create(8, int_hash, int_cmp);
    int v0 = 0, v1 = 1, v2 = 2, v3 = 3;
    size_t component_ids[4];
    size_t parent[4];
    struct visit_ctx ctx;

    printf("graph_test: create\n");
    assert(graph != NULL);

    algokit_graph_add_vertex(graph, &v0);
    algokit_graph_add_vertex(graph, &v1);
    algokit_graph_add_vertex(graph, &v2);
    algokit_graph_add_vertex(graph, &v3);

    algokit_graph_add_edge(graph, &v0, &v1, 2.0);
    algokit_graph_add_edge(graph, &v1, &v2, 1.0);

    printf("graph_test: bfs\n");
    ctx.count = 0;
    assert(algokit_graph_bfs(graph, &v0, count_visit, &ctx) == 0);
    assert(ctx.count == 3);

    printf("graph_test: dfs\n");
    ctx.count = 0;
    assert(algokit_graph_dfs(graph, &v0, count_visit, &ctx) == 0);
    assert(ctx.count == 3);

    printf("graph_test: components\n");
    assert(algokit_graph_connected_components(graph, component_ids) == 2);

    printf("graph_test: mst prim\n");
    assert(algokit_graph_mst_prim(graph, &v0, parent) == 1);

    printf("graph_test: mst kruskal\n");
    assert(algokit_graph_mst_kruskal(graph, parent) == 1);

    printf("graph_test: destroy\n");
    algokit_graph_destroy(graph, NULL);
    printf("graph_test: ok\n");
    return 0;
}
