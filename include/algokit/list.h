#ifndef ALGOKIT_LIST_H
#define ALGOKIT_LIST_H

#include <stddef.h>
#include "algokit/item.h"

/*
 * Singly linked list
 * Invariant: head points to first node; tail points to last node or NULL when empty.
 * Complexity: push_front O(1), push_back O(1), pop_front O(1), peek_front O(1).
 * Ownership: list owns only its nodes; user data freed via destroy callback.
 *
 * Usage:
 *   algokit_list *list = algokit_list_create();
 *   Item item = { key_ptr, val_ptr };
 *   algokit_list_push_back(list, item);
 *   algokit_list_pop_front(list, &item);
 *   algokit_list_destroy(list, destroy_cb);
 *
 * Note: this list exposes only front removal/peek; it is not a random-access list.
 */

typedef struct algokit_list algokit_list;

algokit_list *algokit_list_create(void);

void algokit_list_destroy(algokit_list *list, algokit_item_destroy_fn destroy);

int algokit_list_push_front(algokit_list *list, Item item);

int algokit_list_push_back(algokit_list *list, Item item);

int algokit_list_pop_front(algokit_list *list, Item *out);

int algokit_list_peek_front(const algokit_list *list, Item *out);

size_t algokit_list_size(const algokit_list *list);

int algokit_list_is_empty(const algokit_list *list);

#endif /* ALGOKIT_LIST_H */
