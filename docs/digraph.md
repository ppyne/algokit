# Directed graphs

This module provides directed graphs stored as adjacency lists. Graphs are unweighted by default and use user-defined keys for vertices.

## When to use it

- Dependency graphs and task scheduling
- Workflow pipelines with direction
- Cycle detection and topological ordering

## What algokit provides

Directed graphs use user-provided hash and comparison functions to manage vertex keys. Vertex keys are stored by pointer; the graph never copies the key data.

### Functions, parameters, and behavior

- `algokit_digraph_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp)`
  - Creates an empty directed graph with initial capacity for vertices.
  - Parameters: `capacity` must be > 0, `hash` and `cmp` must not be `NULL`.
  - Returns: `algokit_digraph *` or `NULL` on failure.

- `algokit_digraph_destroy(algokit_digraph *graph, algokit_item_destroy_fn destroy)`
  - Frees the graph and adjacency lists.
  - Parameters: `graph` may be `NULL`, `destroy` is called as `destroy(key, NULL)` for each stored key.

- `algokit_digraph_add_vertex(algokit_digraph *graph, void *key)`
  - Adds a new vertex with the given key.
  - Parameters: `graph` and `key` must not be `NULL`.
  - Returns: `0` on success, `1` if key already exists, nonzero on error.

- `algokit_digraph_add_edge(algokit_digraph *graph, const void *from_key, const void *to_key)`
  - Adds a directed edge from `from_key` to `to_key`.
  - Parameters: `graph`, `from_key`, and `to_key` must not be `NULL`.
  - Returns: `0` on success, `1` if a vertex is missing, nonzero on error.

- `algokit_digraph_vertex_count(const algokit_digraph *graph)`
  - Returns the number of vertices (0 if `graph` is `NULL`).

- `algokit_digraph_vertex_key(const algokit_digraph *graph, size_t index)`
  - Returns the key pointer for a vertex index.
  - Parameters: `index` must be in `[0, vertex_count)`.
  - Returns: key pointer or `NULL` if invalid.

- `algokit_digraph_bfs(const algokit_digraph *graph, const void *start_key, algokit_digraph_visit_fn visit, void *ctx)`
  - Breadth-first traversal from `start_key` following outgoing edges.
  - Parameters: `graph`, `start_key`, and `visit` must not be `NULL`.
  - Returns: `0` on success, `1` if start key not found, nonzero on error.

- `algokit_digraph_dfs(const algokit_digraph *graph, const void *start_key, algokit_digraph_visit_fn visit, void *ctx)`
  - Depth-first traversal from `start_key` following outgoing edges.
  - Parameters: `graph`, `start_key`, and `visit` must not be `NULL`.
  - Returns: `0` on success, `1` if start key not found, nonzero on error.

- `algokit_digraph_topological_sort(const algokit_digraph *graph, size_t *order)`
  - Computes a topological ordering of vertices by index.
  - Parameters: `order` must have length `vertex_count`.
  - Returns: `0` on success, `1` if the graph has a cycle, nonzero on error.

- `algokit_digraph_has_cycle(const algokit_digraph *graph, int *out)`
  - Detects whether the graph contains a cycle.
  - Parameters: `out` must not be `NULL`.
  - Returns: `0` on success, nonzero on error.

### Limits and constraints

- Vertex keys must remain valid while stored in the graph.
- Edges require both vertices to exist.
- The graph does not free keys unless a destroy callback is provided.

## Key points

- Topological ordering is only valid for DAGs.
  - Illustration: `algokit_digraph_topological_sort` returns `1` when a cycle exists.
- Cycle detection uses DFS with visit states.
  - Illustration: a back-edge indicates a cycle.
- Traversals follow edge direction.
  - Illustration: BFS/DFS only visit vertices reachable via outgoing edges.

## Examples

Each example has a dedicated directory under `examples/` with a local `Makefile`:

- `examples/digraph_basic/`
- `examples/digraph_topo/`

Every example includes a context introduction and commented code steps. The listings below mirror the sources.

### Example: BFS/DFS

Context: model a directed workflow. We add edges, then run BFS/DFS from the start node.

```c
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
```

### Example: topological sort and cycle detection

Context: model build dependencies. We compute a topological order and verify whether a cycle exists.

```c
#include <stdio.h>
#include <stddef.h>
#include "algokit/digraph.h"

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
```
