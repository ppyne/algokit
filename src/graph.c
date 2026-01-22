#include "algokit/graph.h"
#include "algokit/uf.h"

#include <float.h>
#include <stdlib.h>

struct algokit_graph_edge {
    size_t to;
    double weight;
    struct algokit_graph_edge *next;
};

struct algokit_graph_vertex {
    void *key;
    struct algokit_graph_edge *adj;
};

struct algokit_graph_map_entry {
    void *key;
    size_t index;
    int state;
};

struct algokit_graph_map {
    struct algokit_graph_map_entry *entries;
    size_t capacity;
    size_t size;
    algokit_key_hash_fn hash;
    algokit_key_cmp_fn cmp;
};

struct algokit_graph {
    struct algokit_graph_vertex *vertices;
    size_t size;
    size_t capacity;
    struct algokit_graph_map map;
};

static size_t algokit_graph_map_index(const struct algokit_graph_map *map, const void *key) {
    return map->hash(key) % map->capacity;
}

static int algokit_graph_map_resize(struct algokit_graph_map *map, size_t new_capacity) {
    size_t i;
    struct algokit_graph_map_entry *old_entries = map->entries;
    size_t old_capacity = map->capacity;

    map->entries = (struct algokit_graph_map_entry *)calloc(new_capacity, sizeof(*map->entries));
    if (!map->entries) {
        map->entries = old_entries;
        return -1;
    }

    map->capacity = new_capacity;
    map->size = 0;

    for (i = 0; i < old_capacity; i++) {
        if (old_entries[i].state == 1) {
            size_t idx = algokit_graph_map_index(map, old_entries[i].key);
            while (map->entries[idx].state == 1) {
                idx = (idx + 1) % map->capacity;
            }
            map->entries[idx] = old_entries[i];
            map->entries[idx].state = 1;
            map->size++;
        }
    }

    free(old_entries);
    return 0;
}

static int algokit_graph_map_get(const struct algokit_graph_map *map, const void *key, size_t *out_index) {
    size_t idx;

    if (!map || !key || !out_index) {
        return -1;
    }

    idx = algokit_graph_map_index(map, key);
    while (map->entries[idx].state != 0) {
        if (map->entries[idx].state == 1 && map->cmp(map->entries[idx].key, key) == 0) {
            *out_index = map->entries[idx].index;
            return 0;
        }
        idx = (idx + 1) % map->capacity;
    }

    return 1;
}

static int algokit_graph_map_put(struct algokit_graph_map *map, void *key, size_t index) {
    size_t idx;

    if (!map) {
        return -1;
    }

    if ((map->size + 1) * 10 >= map->capacity * 7) {
        if (algokit_graph_map_resize(map, map->capacity * 2) != 0) {
            return -1;
        }
    }

    idx = algokit_graph_map_index(map, key);
    while (map->entries[idx].state == 1) {
        if (map->cmp(map->entries[idx].key, key) == 0) {
            return 1;
        }
        idx = (idx + 1) % map->capacity;
    }

    map->entries[idx].key = key;
    map->entries[idx].index = index;
    map->entries[idx].state = 1;
    map->size++;
    return 0;
}

static void algokit_graph_free_edges(struct algokit_graph_edge *edge) {
    while (edge) {
        struct algokit_graph_edge *next = edge->next;
        free(edge);
        edge = next;
    }
}

algokit_graph *algokit_graph_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp) {
    algokit_graph *graph;

    if (!hash || !cmp || capacity == 0) {
        return NULL;
    }

    graph = (algokit_graph *)malloc(sizeof(*graph));
    if (!graph) {
        return NULL;
    }

    graph->vertices = (struct algokit_graph_vertex *)calloc(capacity, sizeof(*graph->vertices));
    if (!graph->vertices) {
        free(graph);
        return NULL;
    }

    graph->size = 0;
    graph->capacity = capacity;

    graph->map.entries = (struct algokit_graph_map_entry *)calloc(capacity, sizeof(*graph->map.entries));
    if (!graph->map.entries) {
        free(graph->vertices);
        free(graph);
        return NULL;
    }
    graph->map.capacity = capacity;
    graph->map.size = 0;
    graph->map.hash = hash;
    graph->map.cmp = cmp;

    return graph;
}

void algokit_graph_destroy(algokit_graph *graph, algokit_item_destroy_fn destroy) {
    size_t i;

    if (!graph) {
        return;
    }

    for (i = 0; i < graph->size; i++) {
        if (destroy) {
            destroy(graph->vertices[i].key, NULL);
        }
        algokit_graph_free_edges(graph->vertices[i].adj);
    }

    free(graph->map.entries);
    free(graph->vertices);
    free(graph);
}

int algokit_graph_add_vertex(algokit_graph *graph, void *key) {
    int rc;
    size_t index;
    struct algokit_graph_vertex *new_vertices;

    if (!graph || !key) {
        return -1;
    }

    rc = algokit_graph_map_get(&graph->map, key, &index);
    if (rc == 0) {
        return 1;
    }
    if (rc < 0) {
        return -1;
    }

    if (graph->size == graph->capacity) {
        size_t new_capacity = graph->capacity * 2;
        new_vertices = (struct algokit_graph_vertex *)realloc(graph->vertices, new_capacity * sizeof(*new_vertices));
        if (!new_vertices) {
            return -1;
        }
        graph->vertices = new_vertices;
        for (index = graph->capacity; index < new_capacity; index++) {
            graph->vertices[index].key = NULL;
            graph->vertices[index].adj = NULL;
        }
        graph->capacity = new_capacity;
        if (algokit_graph_map_resize(&graph->map, new_capacity) != 0) {
            return -1;
        }
    }

    graph->vertices[graph->size].key = key;
    graph->vertices[graph->size].adj = NULL;

    if (algokit_graph_map_put(&graph->map, key, graph->size) != 0) {
        return -1;
    }

    graph->size++;
    return 0;
}

int algokit_graph_add_edge(algokit_graph *graph, const void *u_key, const void *v_key, double weight) {
    size_t u;
    size_t v;
    struct algokit_graph_edge *edge_uv;
    struct algokit_graph_edge *edge_vu;

    if (!graph || !u_key || !v_key) {
        return -1;
    }

    if (algokit_graph_map_get(&graph->map, u_key, &u) != 0) {
        return 1;
    }
    if (algokit_graph_map_get(&graph->map, v_key, &v) != 0) {
        return 1;
    }

    edge_uv = (struct algokit_graph_edge *)malloc(sizeof(*edge_uv));
    edge_vu = (struct algokit_graph_edge *)malloc(sizeof(*edge_vu));
    if (!edge_uv || !edge_vu) {
        free(edge_uv);
        free(edge_vu);
        return -1;
    }

    edge_uv->to = v;
    edge_uv->weight = weight;
    edge_uv->next = graph->vertices[u].adj;
    graph->vertices[u].adj = edge_uv;

    edge_vu->to = u;
    edge_vu->weight = weight;
    edge_vu->next = graph->vertices[v].adj;
    graph->vertices[v].adj = edge_vu;

    return 0;
}

size_t algokit_graph_vertex_count(const algokit_graph *graph) {
    return graph ? graph->size : 0;
}

const void *algokit_graph_vertex_key(const algokit_graph *graph, size_t index) {
    if (!graph || index >= graph->size) {
        return NULL;
    }

    return graph->vertices[index].key;
}

int algokit_graph_bfs(const algokit_graph *graph, const void *start_key, algokit_graph_visit_fn visit, void *ctx) {
    size_t start;
    size_t head = 0;
    size_t tail = 0;
    size_t *queue;
    unsigned char *visited;

    if (!graph || !start_key || !visit) {
        return -1;
    }

    if (algokit_graph_map_get(&graph->map, start_key, &start) != 0) {
        return 1;
    }

    queue = (size_t *)malloc(graph->size * sizeof(*queue));
    visited = (unsigned char *)calloc(graph->size, sizeof(*visited));
    if (!queue || !visited) {
        free(queue);
        free(visited);
        return -1;
    }

    visited[start] = 1;
    queue[tail++] = start;

    while (head < tail) {
        size_t v = queue[head++];
        struct algokit_graph_edge *edge = graph->vertices[v].adj;
        visit(graph->vertices[v].key, ctx);
        while (edge) {
            if (!visited[edge->to]) {
                visited[edge->to] = 1;
                queue[tail++] = edge->to;
            }
            edge = edge->next;
        }
    }

    free(queue);
    free(visited);

    return 0;
}

static void algokit_graph_dfs_visit(const struct algokit_graph *graph, size_t v, unsigned char *visited, algokit_graph_visit_fn visit, void *ctx) {
    struct algokit_graph_edge *edge;

    visited[v] = 1;
    visit(graph->vertices[v].key, ctx);

    edge = graph->vertices[v].adj;
    while (edge) {
        if (!visited[edge->to]) {
            algokit_graph_dfs_visit(graph, edge->to, visited, visit, ctx);
        }
        edge = edge->next;
    }
}

int algokit_graph_dfs(const algokit_graph *graph, const void *start_key, algokit_graph_visit_fn visit, void *ctx) {
    size_t start;
    unsigned char *visited;

    if (!graph || !start_key || !visit) {
        return -1;
    }

    if (algokit_graph_map_get(&graph->map, start_key, &start) != 0) {
        return 1;
    }

    visited = (unsigned char *)calloc(graph->size, sizeof(*visited));
    if (!visited) {
        return -1;
    }

    algokit_graph_dfs_visit(graph, start, visited, visit, ctx);

    free(visited);
    return 0;
}

size_t algokit_graph_connected_components(const algokit_graph *graph, size_t *component_ids) {
    size_t i;
    size_t count = 0;
    unsigned char *visited;

    if (!graph || !component_ids) {
        return 0;
    }

    visited = (unsigned char *)calloc(graph->size, sizeof(*visited));
    if (!visited) {
        return 0;
    }

    for (i = 0; i < graph->size; i++) {
        if (!visited[i]) {
            size_t *stack = (size_t *)malloc(graph->size * sizeof(*stack));
            size_t top = 0;
            if (!stack) {
                free(visited);
                return 0;
            }
            visited[i] = 1;
            stack[top++] = i;
            while (top > 0) {
                size_t v = stack[--top];
                struct algokit_graph_edge *edge = graph->vertices[v].adj;
                component_ids[v] = count;
                while (edge) {
                    if (!visited[edge->to]) {
                        visited[edge->to] = 1;
                        stack[top++] = edge->to;
                    }
                    edge = edge->next;
                }
            }
            free(stack);
            count++;
        }
    }

    free(visited);
    return count;
}

int algokit_graph_mst_prim(const algokit_graph *graph, const void *start_key, size_t *parent) {
    size_t start;
    size_t i;
    unsigned char *in_mst;
    double *key;

    if (!graph || !start_key || !parent) {
        return -1;
    }

    if (algokit_graph_map_get(&graph->map, start_key, &start) != 0) {
        return 1;
    }

    in_mst = (unsigned char *)calloc(graph->size, sizeof(*in_mst));
    key = (double *)malloc(graph->size * sizeof(*key));
    if (!in_mst || !key) {
        free(in_mst);
        free(key);
        return -1;
    }

    for (i = 0; i < graph->size; i++) {
        key[i] = DBL_MAX;
        parent[i] = (size_t)-1;
    }
    key[start] = 0.0;

    for (i = 0; i < graph->size; i++) {
        size_t u = (size_t)-1;
        size_t v;
        double min = DBL_MAX;

        for (v = 0; v < graph->size; v++) {
            if (!in_mst[v] && key[v] < min) {
                min = key[v];
                u = v;
            }
        }

        if (u == (size_t)-1) {
            free(in_mst);
            free(key);
            return 1;
        }

        in_mst[u] = 1;

        {
            struct algokit_graph_edge *edge = graph->vertices[u].adj;
            while (edge) {
                if (!in_mst[edge->to] && edge->weight < key[edge->to]) {
                    key[edge->to] = edge->weight;
                    parent[edge->to] = u;
                }
                edge = edge->next;
            }
        }
    }

    free(in_mst);
    free(key);
    return 0;
}

struct algokit_graph_mst_edge {
    size_t u;
    size_t v;
    double w;
};

static int algokit_graph_edge_cmp(const void *a, const void *b) {
    const struct algokit_graph_mst_edge *ea = (const struct algokit_graph_mst_edge *)a;
    const struct algokit_graph_mst_edge *eb = (const struct algokit_graph_mst_edge *)b;
    return (ea->w > eb->w) - (ea->w < eb->w);
}

int algokit_graph_mst_kruskal(const algokit_graph *graph, size_t *parent) {
    size_t i;
    size_t edge_count = 0;
    size_t mst_edges = 0;
    struct algokit_graph_mst_edge *edges;
    struct algokit_graph_edge **mst_adj;
    int disconnected = 0;

    if (!graph || !parent) {
        return -1;
    }

    for (i = 0; i < graph->size; i++) {
        struct algokit_graph_edge *edge = graph->vertices[i].adj;
        while (edge) {
            if (i < edge->to) {
                edge_count++;
            }
            edge = edge->next;
        }
    }

    edges = (struct algokit_graph_mst_edge *)malloc(edge_count * sizeof(*edges));
    if (!edges) {
        return -1;
    }

    edge_count = 0;
    for (i = 0; i < graph->size; i++) {
        struct algokit_graph_edge *edge = graph->vertices[i].adj;
        while (edge) {
            if (i < edge->to) {
                edges[edge_count].u = i;
                edges[edge_count].v = edge->to;
                edges[edge_count].w = edge->weight;
                edge_count++;
            }
            edge = edge->next;
        }
    }

    qsort(edges, edge_count, sizeof(*edges), algokit_graph_edge_cmp);

    mst_adj = (struct algokit_graph_edge **)calloc(graph->size, sizeof(*mst_adj));
    if (!mst_adj) {
        free(edges);
        return -1;
    }

    {
        algokit_uf_wqupc *uf = algokit_uf_wqupc_create(graph->size);
        if (!uf) {
            free(mst_adj);
            free(edges);
            return -1;
        }

        for (i = 0; i < edge_count && mst_edges + 1 < graph->size; i++) {
            size_t ru;
            size_t rv;
            algokit_uf_wqupc_find(uf, edges[i].u, &ru);
            algokit_uf_wqupc_find(uf, edges[i].v, &rv);
            if (ru != rv) {
                algokit_uf_wqupc_union(uf, edges[i].u, edges[i].v);
                {
                    struct algokit_graph_edge *e1 = (struct algokit_graph_edge *)malloc(sizeof(*e1));
                    struct algokit_graph_edge *e2 = (struct algokit_graph_edge *)malloc(sizeof(*e2));
                    if (!e1 || !e2) {
                        free(e1);
                        free(e2);
                        algokit_uf_wqupc_destroy(uf);
                        free(mst_adj);
                        free(edges);
                        return -1;
                    }
                    e1->to = edges[i].v;
                    e1->weight = edges[i].w;
                    e1->next = mst_adj[edges[i].u];
                    mst_adj[edges[i].u] = e1;

                    e2->to = edges[i].u;
                    e2->weight = edges[i].w;
                    e2->next = mst_adj[edges[i].v];
                    mst_adj[edges[i].v] = e2;
                }
                mst_edges++;
            }
        }

        algokit_uf_wqupc_destroy(uf);
    }

    if (graph->size > 0 && mst_edges + 1 < graph->size) {
        disconnected = 1;
    }

    for (i = 0; i < graph->size; i++) {
        parent[i] = (size_t)-1;
    }

    {
        unsigned char *visited = (unsigned char *)calloc(graph->size, sizeof(*visited));
        if (!visited) {
            for (i = 0; i < graph->size; i++) {
                algokit_graph_free_edges(mst_adj[i]);
            }
            free(mst_adj);
            free(edges);
            return -1;
        }

        for (i = 0; i < graph->size; i++) {
            if (!visited[i]) {
                size_t *queue = (size_t *)malloc(graph->size * sizeof(*queue));
                size_t head = 0;
                size_t tail = 0;
                if (!queue) {
                    free(visited);
                    for (i = 0; i < graph->size; i++) {
                        algokit_graph_free_edges(mst_adj[i]);
                    }
                    free(mst_adj);
                    free(edges);
                    return -1;
                }
                visited[i] = 1;
                queue[tail++] = i;
                while (head < tail) {
                    size_t v = queue[head++];
                    struct algokit_graph_edge *edge = mst_adj[v];
                    while (edge) {
                        if (!visited[edge->to]) {
                            visited[edge->to] = 1;
                            parent[edge->to] = v;
                            queue[tail++] = edge->to;
                        }
                        edge = edge->next;
                    }
                }
                free(queue);
            }
        }

        free(visited);
    }

    for (i = 0; i < graph->size; i++) {
        algokit_graph_free_edges(mst_adj[i]);
    }

    free(mst_adj);
    free(edges);
    return disconnected ? 1 : 0;
}
