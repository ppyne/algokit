# Technical Specification – algokit

## 1. Project Goal

**algokit** is a C99 static library providing a coherent collection of classic data structures and algorithms, inspired by Robert Sedgewick’s *Algorithms in C*.

The project aims to deliver:
- reusable and practical algorithmic tools
- clean, original implementations (no code reuse)
- stable and explicit APIs
- full portability across Unix / Unix-like / BSD systems

The library is intended both as a **practical toolkit** and as a **reference implementation**.

---

## 2. Global Constraints

- Language: ISO **C99 only**
- Compilation flags: `-std=c99 -Wall -Wextra -Wpedantic`
- Output format: static library (`libalgokit.a`)
- Target platforms: Linux, BSD, macOS
- No external dependencies

---

## 3. Core Design Principles

### 3.1 Item / Key Model

All structures and algorithms operate on a generic `Item` type:

```c
typedef struct {
    void *key;
    void *val;
} Item;
```

- The library never assumes the concrete type of `key` or `val`
- Semantic meaning is entirely user-defined
- Comparison, hashing and destruction are provided by the user

---

### 3.2 Memory Ownership

- Each structure provides:
  - a creation function (`*_create`)
  - a destruction function (`*_destroy`)
- The library frees **only its internal memory**
- User data (`key`, `val`) is freed **only via explicit user callbacks**
- No implicit or hidden memory management

---

### 3.3 API Stability

- All public structures are opaque
- Public headers define a stable API
- No hidden side effects
- All behavior must be explicitly documented

---

## 4. Recommended Project Layout

```
algokit/
├── include/
│   └── algokit/
│       ├── item.h
│       ├── stack.h
│       ├── queue.h
│       ├── list.h
│       ├── bst.h
│       ├── rbtree.h
│       ├── heap.h
│       ├── uf.h
│       ├── hash.h
│       ├── sort.h
│       ├── graph.h
│       └── digraph.h
├── src/
├── examples/
├── tests/
├── Makefile
├── README.md
├── SPEC.md
└── ROADMAP.md
```

---

## 5. Data Structures

For **each structure**, the following must be provided:

1. Public header with documentation
2. Clean C99 implementation
3. Description of invariants
4. Time and space complexity
5. **At least one concrete, compilable usage example**

---

### 5.1 Linear Structures

- Stack (LIFO)
- Queue (FIFO)
- Linked List (singly or doubly; design choice must be documented)

Required operations:
- create / destroy
- insert / remove
- access / peek
- size / empty

---

### 5.2 Union-Find (Disjoint Set)

Implementations:
- Quick-Find
- Quick-Union
- Weighted Quick-Union with path compression

Operations:
- create
- union
- find
- connected

---

### 5.3 Heap

- Binary heap (max-heap by default)

Operations:
- insert
- extract
- peek
- size

---

### 5.4 Search Trees

- Binary Search Tree (BST)
- Red-Black Tree

Operations:
- insert
- search
- delete
- traversals (inorder, preorder, postorder)

---

### 5.5 Hash Tables

Implementations:
- Linear Probing
- Separate Chaining

Operations:
- put
- get
- delete
- dynamic resize

---

### 5.6 Graphs

Undirected weighted graphs (current scope):
- Representation: adjacency lists storing `(neighbor, weight)` entries
- Vertex identifiers are user-defined keys, managed through the Item/Key model
- Edge weights are `double`
- Algorithms: BFS, DFS, connected components, MST (Prim and Kruskal)

Directed graphs (future scope; separate APIs):
- Representation: adjacency lists
- Vertex identifiers are user-defined keys, managed through the Item/Key model
- Unweighted by default (weights optional if justified)
- Algorithms: BFS, DFS, topological sort, cycle detection
- No MST for directed graphs

---

## 6. Sorting Algorithms

Sorting algorithms operate on arrays of `Item`.

Algorithms:
- Insertion sort
- Shell sort
- Quick sort
- Merge sort
- Heap sort

Each algorithm must document:
- algorithmic principle
- time and space complexity
- minimal usage example

---

## 7. Documentation Requirements

For each module:
- description and purpose
- invariants
- complexity analysis
- assumptions and constraints
- example usage code

Additionally:
- Each structure must have a dedicated `docs/<name>.md` page.
- Documentation is the highest priority deliverable and must be in English, written for novice developers.
- Documentation must be high-level with full sentences explaining each concept.
- Under each "What algokit provides" or "Key points" section, include concrete illustrations:
  - enumerate every function, its parameters, and any relevant variables/structures
  - explain how the concepts and functions operate, which parameters are used, and any limits
  - include examples that demonstrate every listed capability and key point
- All API functions must be documented with a short descriptive sentence, plus what each function does, its parameters, and its return values.
- Each example must include:
  - a short introduction explaining the context, use case, data being manipulated, and why
  - code comments describing what happens at each key step
- Examples must be practical and demonstrate all available options/capabilities.
- Each example must live in its own subdirectory under `examples/` with its own `Makefile` showing how to compile it.
- The root `examples/Makefile` must build all examples and provide a `test` target that runs them.
- Tests must include running each example via its `Makefile` so examples are exercised.
- README must include a "Documentation" section placed before "Build".
- README "Documentation" section must use Markdown links with short descriptions
  explaining what each page covers for a human reader.

---

## 8. Examples

The `examples/` directory must contain:
- one standalone program per structure or algorithm
- code compilable via the provided Makefile
- realistic usage scenarios (dictionary, priority queue, graph traversal, etc.)

---

## 9. Testing

- simple unit tests (assert-based)
- regression tests
- memory correctness (Valgrind-compatible)

---

## 10. Expected Output from Codex

Codex is expected to deliver:
- fully compilable C99 code
- documented public headers
- consistent and original implementations
- concrete usage examples
- strict adherence to this specification

Any ambiguity must be reported explicitly rather than silently assumed.
