# Development Roadmap – algokit

This roadmap breaks the project into **coherent, incremental phases**, each of which can be assigned independently to Codex. Every phase produces usable and verifiable code.

---

## Phase 0 – Foundations (mandatory)

### Goals
- Establish common foundations for the entire library
- Prevent long-term technical debt

### Deliverables
- `include/algokit/item.h`
- global conventions (naming, error handling, memory ownership)
- working Makefile producing `libalgokit.a`
- minimal example demonstrating compilation and linkage

### Validation
- compilation with zero warnings
- successful linkage with a user program

---

## Phase 1 – Linear Data Structures

### Modules
- Stack
- Queue
- Linked List

### For each module
- documented public header
- clean implementation
- concrete example (`examples/stack_basic/`, etc.)
- simple tests

### Validation
- intensive usage (push/pop loops)
- memory correctness checks
- examples must each compile via their own `Makefile` and be runnable from `examples/Makefile`

---

## Phase 2 – Union-Find (Canonical ADT)

### Implementations
- Quick-Find
- Quick-Union
- Weighted Quick-Union with path compression

### Goals
- validate the Item / Key model
- validate API conventions

### Validation
- example: connected components detection

---

## Phase 3 – Sorting Algorithms

### Algorithms
- Insertion sort
- Shell sort
- Quick sort
- Merge sort
- Heap sort

### Notes
- sorting arrays of `Item`
- user-provided comparison function

### Validation
- sorting complex structures
- optional performance comparison

---

## Phase 4 – Heap

### Type
- Binary heap (max-heap by default)

### Goals
- direct support for Heap Sort
- priority queue / scheduling use cases

### Validation
- priority queue example

---

## Phase 5 – Search Trees

### Modules
- Binary Search Tree (BST)
- Red-Black Tree

### Algorithms
- insert
- search
- delete
- traversals

### Validation
- dictionary key/value example
- Red-Black invariants verification

---

## Phase 6 – Hash Tables

### Implementations
- Linear Probing
- Separate Chaining

### Goals
- strategy comparison
- dynamic resizing

### Validation
- simple benchmark
- deliberate collision tests

---

## Phase 7 – Graphs (Undirected, Weighted)

### Representation
- adjacency lists storing `(neighbor, weight)` entries
- vertex identifiers via Item/Key model

### Algorithms
- BFS
- DFS
- connected components
- minimum spanning tree (Prim and Kruskal)

### Validation
- small real-world graph
- textual visualization

---

## Phase 8 – Directed Graphs (Separate APIs)

### Representation
- adjacency lists
- unweighted by default (weights optional if justified)
- vertex identifiers via Item/Key model

### Algorithms
- BFS
- DFS
- topological sort
- cycle detection

### Notes
- no MST for directed graphs

---

## Phase 9 – Consolidation

### Actions
- documentation harmonization
- API review
- code cleanup
- global README update

### Optional
- benchmarks
- debug instrumentation

---

## Recommended Workflow with Codex

1. Provide **the specification + one specific phase**
2. Require: code + documentation + examples
3. Manually review and validate
4. Move to the next phase

Never assign multiple phases at once.

---

## Final Expected Outcome

- stable `libalgokit.a`
- clear and documented API
- complete and coherent algorithmic corpus
- usable as both a professional tool and a reference implementation
