#ifndef ALGOKIT_SORT_H
#define ALGOKIT_SORT_H

#include <stddef.h>
#include "algokit/item.h"

/*
 * Sorting algorithms for arrays of Item.
 * Each function sorts in-place using the user-provided key comparison.
 *
 * Parameters:
 * - items: array of Item values
 * - n: number of elements in the array
 * - cmp: key comparison function (must not be NULL)
 *
 * Returns:
 * - 0 on success
 * - nonzero on error (e.g., invalid parameters, allocation failure)
 */

int algokit_sort_insertion(Item *items, size_t n, algokit_key_cmp_fn cmp);
int algokit_sort_shell(Item *items, size_t n, algokit_key_cmp_fn cmp);
int algokit_sort_quick(Item *items, size_t n, algokit_key_cmp_fn cmp);
int algokit_sort_merge(Item *items, size_t n, algokit_key_cmp_fn cmp);
int algokit_sort_heap(Item *items, size_t n, algokit_key_cmp_fn cmp);

#endif /* ALGOKIT_SORT_H */
