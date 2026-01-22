#include "algokit/digraph.h"

#include <stdlib.h>

struct algokit_digraph_edge {
    size_t to;
    struct algokit_digraph_edge *next;
};

struct algokit_digraph_vertex {
    void *key;
    struct algokit_digraph_edge *adj;
};

struct algokit_digraph_map_entry {
    void *key;
    size_t index;
    int state;
};

struct algokit_digraph_map {
    struct algokit_digraph_map_entry *entries;
    size_t capacity;
    size_t size;
    algokit_key_hash_fn hash;
    algokit_key_cmp_fn cmp;
};

struct algokit_digraph {
    struct algokit_digraph_vertex *vertices;
    size_t size;
    size_t capacity;
    struct algokit_digraph_map map;
};

static size_t algokit_digraph_map_index(const struct algokit_digraph_map *map, const void *key) {
    return map->hash(key) % map->capacity;
}

static int algokit_digraph_map_resize(struct algokit_digraph_map *map, size_t new_capacity) {
    size_t i;
    struct algokit_digraph_map_entry *old_entries = map->entries;
    size_t old_capacity = map->capacity;

    map->entries = (struct algokit_digraph_map_entry *)calloc(new_capacity, sizeof(*map->entries));
    if (!map->entries) {
        map->entries = old_entries;
        return -1;
    }

    map->capacity = new_capacity;
    map->size = 0;

    for (i = 0; i < old_capacity; i++) {
        if (old_entries[i].state == 1) {
            size_t idx = algokit_digraph_map_index(map, old_entries[i].key);
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

static int algokit_digraph_map_get(const struct algokit_digraph_map *map, const void *key, size_t *out_index) {
    size_t idx;

    if (!map || !key || !out_index) {
        return -1;
    }

    idx = algokit_digraph_map_index(map, key);
    while (map->entries[idx].state != 0) {
        if (map->entries[idx].state == 1 && map->cmp(map->entries[idx].key, key) == 0) {
            *out_index = map->entries[idx].index;
            return 0;
        }
        idx = (idx + 1) % map->capacity;
    }

    return 1;
}

static int algokit_digraph_map_put(struct algokit_digraph_map *map, void *key, size_t index) {
    size_t idx;

    if (!map) {
        return -1;
    }

    if ((map->size + 1) * 10 >= map->capacity * 7) {
        if (algokit_digraph_map_resize(map, map->capacity * 2) != 0) {
            return -1;
        }
    }

    idx = algokit_digraph_map_index(map, key);
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

static void algokit_digraph_free_edges(struct algokit_digraph_edge *edge) {
    while (edge) {
        struct algokit_digraph_edge *next = edge->next;
        free(edge);
        edge = next;
    }
}

algokit_digraph *algokit_digraph_create(size_t capacity, algokit_key_hash_fn hash, algokit_key_cmp_fn cmp) {
    algokit_digraph *graph;

    if (!hash || !cmp || capacity == 0) {
        return NULL;
    }

    graph = (algokit_digraph *)malloc(sizeof(*graph));
    if (!graph) {
        return NULL;
    }

    graph->vertices = (struct algokit_digraph_vertex *)calloc(capacity, sizeof(*graph->vertices));
    if (!graph->vertices) {
        free(graph);
        return NULL;
    }

    graph->size = 0;
    graph->capacity = capacity;

    graph->map.entries = (struct algokit_digraph_map_entry *)calloc(capacity, sizeof(*graph->map.entries));
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

void algokit_digraph_destroy(algokit_digraph *graph, algokit_item_destroy_fn destroy) {
    size_t i;

    if (!graph) {
        return;
    }

    for (i = 0; i < graph->size; i++) {
        if (destroy) {
            destroy(graph->vertices[i].key, NULL);
        }
        algokit_digraph_free_edges(graph->vertices[i].adj);
    }

    free(graph->map.entries);
    free(graph->vertices);
    free(graph);
}

int algokit_digraph_add_vertex(algokit_digraph *graph, void *key) {
    int rc;
    size_t index;
    struct algokit_digraph_vertex *new_vertices;

    if (!graph || !key) {
        return -1;
    }

    rc = algokit_digraph_map_get(&graph->map, key, &index);
    if (rc == 0) {
        return 1;
    }
    if (rc < 0) {
        return -1;
    }

    if (graph->size == graph->capacity) {
        size_t new_capacity = graph->capacity * 2;
        new_vertices = (struct algokit_digraph_vertex *)realloc(graph->vertices, new_capacity * sizeof(*new_vertices));
        if (!new_vertices) {
            return -1;
        }
        graph->vertices = new_vertices;
        for (index = graph->capacity; index < new_capacity; index++) {
            graph->vertices[index].key = NULL;
            graph->vertices[index].adj = NULL;
        }
        graph->capacity = new_capacity;
        if (algokit_digraph_map_resize(&graph->map, new_capacity) != 0) {
            return -1;
        }
    }

    graph->vertices[graph->size].key = key;
    graph->vertices[graph->size].adj = NULL;

    if (algokit_digraph_map_put(&graph->map, key, graph->size) != 0) {
        return -1;
    }

    graph->size++;
    return 0;
}

int algokit_digraph_add_edge(algokit_digraph *graph, const void *from_key, const void *to_key) {
    size_t from;
    size_t to;
    struct algokit_digraph_edge *edge;

    if (!graph || !from_key || !to_key) {
        return -1;
    }

    if (algokit_digraph_map_get(&graph->map, from_key, &from) != 0) {
        return 1;
    }
    if (algokit_digraph_map_get(&graph->map, to_key, &to) != 0) {
        return 1;
    }

    edge = (struct algokit_digraph_edge *)malloc(sizeof(*edge));
    if (!edge) {
        return -1;
    }

    edge->to = to;
    edge->next = graph->vertices[from].adj;
    graph->vertices[from].adj = edge;
    return 0;
}

size_t algokit_digraph_vertex_count(const algokit_digraph *graph) {
    return graph ? graph->size : 0;
}

const void *algokit_digraph_vertex_key(const algokit_digraph *graph, size_t index) {
    if (!graph || index >= graph->size) {
        return NULL;
    }

    return graph->vertices[index].key;
}

int algokit_digraph_bfs(const algokit_digraph *graph, const void *start_key, algokit_digraph_visit_fn visit, void *ctx) {
    size_t start;
    size_t head = 0;
    size_t tail = 0;
    size_t *queue;
    unsigned char *visited;

    if (!graph || !start_key || !visit) {
        return -1;
    }

    if (algokit_digraph_map_get(&graph->map, start_key, &start) != 0) {
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
        struct algokit_digraph_edge *edge = graph->vertices[v].adj;
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

static void algokit_digraph_dfs_visit(const algokit_digraph *graph, size_t v, unsigned char *visited, algokit_digraph_visit_fn visit, void *ctx) {
    struct algokit_digraph_edge *edge;

    visited[v] = 1;
    visit(graph->vertices[v].key, ctx);

    edge = graph->vertices[v].adj;
    while (edge) {
        if (!visited[edge->to]) {
            algokit_digraph_dfs_visit(graph, edge->to, visited, visit, ctx);
        }
        edge = edge->next;
    }
}

int algokit_digraph_dfs(const algokit_digraph *graph, const void *start_key, algokit_digraph_visit_fn visit, void *ctx) {
    size_t start;
    unsigned char *visited;

    if (!graph || !start_key || !visit) {
        return -1;
    }

    if (algokit_digraph_map_get(&graph->map, start_key, &start) != 0) {
        return 1;
    }

    visited = (unsigned char *)calloc(graph->size, sizeof(*visited));
    if (!visited) {
        return -1;
    }

    algokit_digraph_dfs_visit(graph, start, visited, visit, ctx);
    free(visited);
    return 0;
}

int algokit_digraph_topological_sort(const algokit_digraph *graph, size_t *order) {
    size_t i;
    size_t head = 0;
    size_t tail = 0;
    size_t *queue;
    size_t *in_degree;
    size_t count = 0;

    if (!graph || !order) {
        return -1;
    }

    queue = (size_t *)malloc(graph->size * sizeof(*queue));
    in_degree = (size_t *)calloc(graph->size, sizeof(*in_degree));
    if (!queue || !in_degree) {
        free(queue);
        free(in_degree);
        return -1;
    }

    for (i = 0; i < graph->size; i++) {
        struct algokit_digraph_edge *edge = graph->vertices[i].adj;
        while (edge) {
            in_degree[edge->to]++;
            edge = edge->next;
        }
    }

    for (i = 0; i < graph->size; i++) {
        if (in_degree[i] == 0) {
            queue[tail++] = i;
        }
    }

    while (head < tail) {
        size_t v = queue[head++];
        struct algokit_digraph_edge *edge = graph->vertices[v].adj;
        order[count++] = v;
        while (edge) {
            in_degree[edge->to]--;
            if (in_degree[edge->to] == 0) {
                queue[tail++] = edge->to;
            }
            edge = edge->next;
        }
    }

    free(queue);
    free(in_degree);

    return count == graph->size ? 0 : 1;
}

static int algokit_digraph_cycle_dfs(const struct algokit_digraph *graph, size_t v, unsigned char *state) {
    struct algokit_digraph_edge *edge;

    state[v] = 1;
    edge = graph->vertices[v].adj;
    while (edge) {
        if (state[edge->to] == 1) {
            return 1;
        }
        if (state[edge->to] == 0 && algokit_digraph_cycle_dfs(graph, edge->to, state)) {
            return 1;
        }
        edge = edge->next;
    }
    state[v] = 2;
    return 0;
}

int algokit_digraph_has_cycle(const algokit_digraph *graph, int *out) {
    size_t i;
    unsigned char *state;

    if (!graph || !out) {
        return -1;
    }

    state = (unsigned char *)calloc(graph->size, sizeof(*state));
    if (!state) {
        return -1;
    }

    for (i = 0; i < graph->size; i++) {
        if (state[i] == 0) {
            if (algokit_digraph_cycle_dfs(graph, i, state)) {
                *out = 1;
                free(state);
                return 0;
            }
        }
    }

    *out = 0;
    free(state);
    return 0;
}
