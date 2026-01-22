# Graphs (undirected, weighted)

This module provides undirected, weighted graphs stored as adjacency lists. Each adjacency entry stores a `(neighbor, weight)` pair, and vertices are identified by user-defined keys.

## When to use it

- Model relationships between entities (roads, networks, dependencies)
- Run BFS/DFS for reachability
- Compute connected components or minimum spanning trees

## What algokit provides

Graphs use user-provided hash and comparison functions to manage vertex keys. Vertex keys are stored by pointer; the graph never copies the key data.

### Functions, parameters, and behavior

- `algokit_graph_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp)`
  - Creates an empty graph with initial capacity for vertices.
  - Parameters: `capacity` must be > 0, `hash` and `cmp` must not be `NULL`.
  - Returns: `algokit_graph *` or `NULL` on failure.

- `algokit_graph_destroy(algokit_graph *graph, algokit_item_destroy_fn destroy)`
  - Frees the graph and adjacency lists.
  - Parameters: `graph` may be `NULL`, `destroy` is called as `destroy(key, NULL)` for each stored key.

- `algokit_graph_add_vertex(algokit_graph *graph, void *key)`
  - Adds a new vertex with the given key.
  - Parameters: `graph` and `key` must not be `NULL`.
  - Returns: `0` on success, `1` if key already exists, nonzero on error.

- `algokit_graph_add_edge(algokit_graph *graph, const void *u_key, const void *v_key, double weight)`
  - Adds an undirected weighted edge between two existing vertices.
  - Parameters: `graph`, `u_key`, and `v_key` must not be `NULL`.
  - Returns: `0` on success, `1` if a vertex is missing, nonzero on error.

- `algokit_graph_vertex_count(const algokit_graph *graph)`
  - Returns the number of vertices (0 if `graph` is `NULL`).

- `algokit_graph_vertex_key(const algokit_graph *graph, size_t index)`
  - Returns the key pointer for a vertex index.
  - Parameters: `index` must be in `[0, vertex_count)`.
  - Returns: key pointer or `NULL` if invalid.

- `algokit_graph_bfs(const algokit_graph *graph, const void *start_key, algokit_graph_visit_fn visit, void *ctx)`
  - Breadth-first traversal from `start_key`.
  - Parameters: `graph`, `start_key`, and `visit` must not be `NULL`.
  - Returns: `0` on success, `1` if start key not found, nonzero on error.

- `algokit_graph_dfs(const algokit_graph *graph, const void *start_key, algokit_graph_visit_fn visit, void *ctx)`
  - Depth-first traversal from `start_key`.
  - Parameters: `graph`, `start_key`, and `visit` must not be `NULL`.
  - Returns: `0` on success, `1` if start key not found, nonzero on error.

- `algokit_graph_connected_components(const algokit_graph *graph, size_t *component_ids)`
  - Computes connected components; writes a component id for each vertex index.
  - Parameters: `component_ids` must have length `vertex_count`.
  - Returns: number of components (0 on error).

- `algokit_graph_mst_prim(const algokit_graph *graph, const void *start_key, size_t *parent)`
  - Computes a minimum spanning tree using Prim from `start_key`.
  - Parameters: `parent` must have length `vertex_count`.
  - Returns: `0` if the graph is connected, `1` if disconnected, nonzero on error.

- `algokit_graph_mst_kruskal(const algokit_graph *graph, size_t *parent)`
  - Computes a minimum spanning forest using Kruskal.
  - Parameters: `parent` must have length `vertex_count`.
  - Returns: `0` if the graph is connected, `1` if disconnected, nonzero on error.

### Limits and constraints

- Vertex keys must remain valid while stored in the graph.
- Edges require both vertices to exist.
- The graph does not free keys unless a destroy callback is provided.

## Key points

- BFS/DFS operate on keys but traverse internal indices.
  - Illustration: `algokit_graph_vertex_key` maps index back to a key.
- Connected components return ids by index, not by key.
  - Illustration: use `algokit_graph_vertex_key` to match ids to keys.
- MST routines return parent indices for the tree/forest.
  - Illustration: parent index `SIZE_MAX` indicates a root.

## Examples

Each example has a dedicated directory under `examples/` with a local `Makefile`:

- `examples/graph_basic/`
- `examples/graph_mst/`

Every example includes a context introduction and commented code steps. The listings below mirror the sources.

### Example: BFS/DFS and connected components

Context: model a small network of cities (keys are integers). We add weighted edges, run BFS/DFS, and compute connected components.

```c
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

    /* Destroy the graph; keys are stack-allocated, so no destroy callback. */
    algokit_graph_destroy(graph, NULL);
    return 0;
}
```

### Example: MST (Prim and Kruskal)

Context: model a weighted network where we want the minimum spanning tree. We compute MSTs using Prim and Kruskal and print parent links by key.

```c
#include <stdio.h>
#include <stddef.h>
#include "algokit/graph.h"

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
```
