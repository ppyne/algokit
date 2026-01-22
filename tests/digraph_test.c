#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "algokit/digraph.h"

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

static size_t find_pos(const algokit_digraph *graph, const size_t *order, size_t n, int key_value) {
    size_t i;
    for (i = 0; i < n; i++) {
        const int *key = (const int *)algokit_digraph_vertex_key(graph, order[i]);
        if (*key == key_value) {
            return i;
        }
    }
    return n;
}

int main(void) {
    algokit_digraph *graph = algokit_digraph_create(8, int_hash, int_cmp);
    int v0 = 0, v1 = 1, v2 = 2, v3 = 3;
    size_t order[4];
    int has_cycle;
    struct visit_ctx ctx;
    size_t pos0, pos1, pos2, pos3;

    printf("digraph_test: create\n");
    assert(graph != NULL);

    algokit_digraph_add_vertex(graph, &v0);
    algokit_digraph_add_vertex(graph, &v1);
    algokit_digraph_add_vertex(graph, &v2);
    algokit_digraph_add_vertex(graph, &v3);

    algokit_digraph_add_edge(graph, &v0, &v1);
    algokit_digraph_add_edge(graph, &v1, &v2);
    algokit_digraph_add_edge(graph, &v0, &v3);

    printf("digraph_test: bfs\n");
    ctx.count = 0;
    assert(algokit_digraph_bfs(graph, &v0, count_visit, &ctx) == 0);
    assert(ctx.count == 4);

    printf("digraph_test: dfs\n");
    ctx.count = 0;
    assert(algokit_digraph_dfs(graph, &v0, count_visit, &ctx) == 0);
    assert(ctx.count == 4);

    printf("digraph_test: topo\n");
    assert(algokit_digraph_topological_sort(graph, order) == 0);
    pos0 = find_pos(graph, order, 4, 0);
    pos1 = find_pos(graph, order, 4, 1);
    pos2 = find_pos(graph, order, 4, 2);
    pos3 = find_pos(graph, order, 4, 3);
    assert(pos0 < pos1);
    assert(pos1 < pos2);
    assert(pos0 < pos3);

    printf("digraph_test: cycle\n");
    assert(algokit_digraph_has_cycle(graph, &has_cycle) == 0);
    assert(has_cycle == 0);

    printf("digraph_test: cycle add edge\n");
    algokit_digraph_add_edge(graph, &v2, &v0);
    assert(algokit_digraph_has_cycle(graph, &has_cycle) == 0);
    assert(has_cycle == 1);

    printf("digraph_test: destroy\n");
    algokit_digraph_destroy(graph, NULL);
    printf("digraph_test: ok\n");
    return 0;
}
