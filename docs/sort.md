# Sorting algorithms

This module provides classic sorting algorithms that operate on arrays of `Item`. Each algorithm sorts in-place using a user-provided key comparison function.

## When to use it

- Sort records by a key while keeping a value attached
- Compare algorithm behavior (simplicity vs performance)
- Teach or study classic sorting techniques

## What algokit provides

All functions take the same inputs:

- `items`: array of `Item` to sort
- `n`: number of elements
- `cmp`: key comparison function for `Item.key`

All functions return `0` on success and nonzero on error (e.g., invalid parameters or allocation failure in merge sort).

### API functions, parameters, and behavior

- `algokit_sort_insertion(Item *items, size_t n, algokit_key_cmp_fn cmp)`
  - Performs insertion sort on the array.
  - Parameters: `items` must be non-NULL when `n > 0`, `cmp` must not be `NULL`.
  - Returns: `0` on success, nonzero on error.

- `algokit_sort_shell(Item *items, size_t n, algokit_key_cmp_fn cmp)`
  - Performs shell sort using a gap sequence.
  - Parameters: `items` must be non-NULL when `n > 0`, `cmp` must not be `NULL`.
  - Returns: `0` on success, nonzero on error.

- `algokit_sort_quick(Item *items, size_t n, algokit_key_cmp_fn cmp)`
  - Performs quick sort with in-place partitioning.
  - Parameters: `items` must be non-NULL when `n > 0`, `cmp` must not be `NULL`.
  - Returns: `0` on success, nonzero on error.

- `algokit_sort_merge(Item *items, size_t n, algokit_key_cmp_fn cmp)`
  - Performs merge sort using a temporary buffer.
  - Parameters: `items` must be non-NULL when `n > 0`, `cmp` must not be `NULL`.
  - Returns: `0` on success, nonzero on error (e.g., allocation failure).

- `algokit_sort_heap(Item *items, size_t n, algokit_key_cmp_fn cmp)`
  - Performs heap sort using a max-heap.
  - Parameters: `items` must be non-NULL when `n > 0`, `cmp` must not be `NULL`.
  - Returns: `0` on success, nonzero on error.

### Complexity overview

- Insertion sort: O(n^2) time, O(1) extra space
- Shell sort: depends on gap sequence, typically better than O(n^2)
- Quick sort: average O(n log n), worst-case O(n^2)
- Merge sort: O(n log n) time, O(n) extra space
- Heap sort: O(n log n) time, O(1) extra space

## Key points

- Sorting operates on `Item.key`, but the `Item.val` travels with the key.
  - Illustration: sorting by key reorders records while keeping values attached.
- The comparison function defines ordering; the library never inspects key types.
  - Illustration: you can sort integers, strings, or structs by providing a custom comparator.
- Merge sort allocates a temporary buffer; other sorts do not allocate.
  - Illustration: a failed allocation returns an error for merge sort only.

## Examples

Each algorithm has a dedicated example directory under `examples/` with a local `Makefile`:

- `examples/sort_insertion/`
- `examples/sort_shell/`
- `examples/sort_quick/`
- `examples/sort_merge/`
- `examples/sort_heap/`

Below are the example listings as documented references. Each includes context and key-step comments.

### Example: insertion sort

Context: sort a small batch of scores where simplicity matters. We keep labels attached to their keys.

```c
#include <stdio.h>
#include "algokit/sort.h"

static int int_key_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void print_items(const Item *items, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%d:%s\n", *(int *)items[i].key, (const char *)items[i].val);
    }
}

int main(void) {
    int keys[] = {5, 1, 4, 2, 3};
    const char *labels[] = {"e", "a", "d", "b", "c"};
    Item items[5];
    size_t i;

    /* Build the Item array so each key travels with its label. */
    for (i = 0; i < 5; i++) {
        items[i].key = &keys[i];
        items[i].val = (void *)labels[i];
    }

    /* Show the original order. */
    printf("before:\n");
    print_items(items, 5);

    /* Sort in-place with insertion sort using the key comparator. */
    if (algokit_sort_insertion(items, 5, int_key_cmp) != 0) {
        return 1;
    }

    /* Show the sorted result. */
    printf("after:\n");
    print_items(items, 5);

    return 0;
}
```

### Example: shell sort

Context: sort a medium list of IDs in-place with a faster method than insertion sort.

```c
#include <stdio.h>
#include "algokit/sort.h"

static int int_key_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void print_items(const Item *items, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%d:%s\n", *(int *)items[i].key, (const char *)items[i].val);
    }
}

int main(void) {
    int keys[] = {12, 7, 19, 3, 5, 14};
    const char *labels[] = {"l", "g", "s", "c", "e", "n"};
    Item items[6];
    size_t i;

    /* Build the Item array so each key travels with its label. */
    for (i = 0; i < 6; i++) {
        items[i].key = &keys[i];
        items[i].val = (void *)labels[i];
    }

    /* Show the original order. */
    printf("before:\n");
    print_items(items, 6);

    /* Sort in-place with shell sort using the key comparator. */
    if (algokit_sort_shell(items, 6, int_key_cmp) != 0) {
        return 1;
    }

    /* Show the sorted result. */
    printf("after:\n");
    print_items(items, 6);

    return 0;
}
```

### Example: quick sort

Context: sort transaction amounts quickly for reporting while keeping labels attached.

```c
#include <stdio.h>
#include "algokit/sort.h"

static int int_key_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void print_items(const Item *items, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%d:%s\n", *(int *)items[i].key, (const char *)items[i].val);
    }
}

int main(void) {
    int keys[] = {42, 7, 19, 7, 3, 25};
    const char *labels[] = {"x", "b", "s", "b2", "c", "y"};
    Item items[6];
    size_t i;

    /* Build the Item array so each key travels with its label. */
    for (i = 0; i < 6; i++) {
        items[i].key = &keys[i];
        items[i].val = (void *)labels[i];
    }

    /* Show the original order. */
    printf("before:\n");
    print_items(items, 6);

    /* Sort in-place with quick sort using the key comparator. */
    if (algokit_sort_quick(items, 6, int_key_cmp) != 0) {
        return 1;
    }

    /* Show the sorted result. */
    printf("after:\n");
    print_items(items, 6);

    return 0;
}
```

### Example: merge sort

Context: sort records where predictable performance matters, using merge sort on key/value pairs.

```c
#include <stdio.h>
#include "algokit/sort.h"

static int int_key_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void print_items(const Item *items, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%d:%s\n", *(int *)items[i].key, (const char *)items[i].val);
    }
}

int main(void) {
    int keys[] = {9, 1, 8, 2, 7, 3};
    const char *labels[] = {"i", "a", "h", "b", "g", "c"};
    Item items[6];
    size_t i;

    /* Build the Item array so each key travels with its label. */
    for (i = 0; i < 6; i++) {
        items[i].key = &keys[i];
        items[i].val = (void *)labels[i];
    }

    /* Show the original order. */
    printf("before:\n");
    print_items(items, 6);

    /* Sort in-place with merge sort using the key comparator. */
    if (algokit_sort_merge(items, 6, int_key_cmp) != 0) {
        return 1;
    }

    /* Show the sorted result. */
    printf("after:\n");
    print_items(items, 6);

    return 0;
}
```

### Example: heap sort

Context: sort a batch of priority values where O(1) extra space matters.

```c
#include <stdio.h>
#include "algokit/sort.h"

static int int_key_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static void print_items(const Item *items, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%d:%s\n", *(int *)items[i].key, (const char *)items[i].val);
    }
}

int main(void) {
    int keys[] = {6, 4, 10, 2, 8};
    const char *labels[] = {"f", "d", "j", "b", "h"};
    Item items[5];
    size_t i;

    /* Build the Item array so each key travels with its label. */
    for (i = 0; i < 5; i++) {
        items[i].key = &keys[i];
        items[i].val = (void *)labels[i];
    }

    /* Show the original order. */
    printf("before:\n");
    print_items(items, 5);

    /* Sort in-place with heap sort using the key comparator. */
    if (algokit_sort_heap(items, 5, int_key_cmp) != 0) {
        return 1;
    }

    /* Show the sorted result. */
    printf("after:\n");
    print_items(items, 5);

    return 0;
}
```
