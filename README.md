# algokit

algokit is a C99 algorithm and data structure toolkit for Unix-like systems, offering reusable, well-documented implementations inspired by classic algorithmic literature.

## Documentation

- [Docs Index](docs/README.md) – Overview and links to all module documentation.
- [Stack](docs/stack.md) – LIFO behavior, API overview, and examples (integers + key/value).
- [Queue](docs/queue.md) – FIFO behavior, API overview, and examples (integers + key/value).
- [List](docs/list.md) – Singly linked list usage, API overview, and examples (integers + key/value).
- [Union-Find](docs/uf.md) – Disjoint set variants, API overview, and connected-components examples.
- [Sorting](docs/sort.md) – Sorting algorithms, API overview, and example locations.
- [Heap](docs/heap.md) – Binary max-heap API, behavior, and example usage.
- [BST](docs/bst.md) – Binary search tree API, traversal behavior, and example usage.
- [Red-Black Tree](docs/rbtree.md) – Balanced tree API, invariants, and example usage.
- [Hash Tables](docs/hash.md) – Linear probing and separate chaining APIs with examples.
- [Graphs](docs/graph.md) – Undirected weighted graphs, traversals, and MST examples.
- [Directed Graphs](docs/digraph.md) – Directed graph algorithms, topological sort, and cycle detection.

## Build

```sh
make
```

## Examples

```sh
make examples
```

Each example lives in its own subdirectory under `examples/` with a local `Makefile`.
Binaries are produced inside those subdirectories (for example, `examples/stack_basic/stack_basic`).

## Tests

```sh
make test
```

Test binaries are produced in `tests/` and executed by `make test`.
