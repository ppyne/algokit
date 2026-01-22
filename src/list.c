#include "algokit/list.h"

#include <stdlib.h>

struct algokit_list_node {
    Item item;
    struct algokit_list_node *next;
};

struct algokit_list {
    struct algokit_list_node *head;
    struct algokit_list_node *tail;
    size_t size;
};

algokit_list *algokit_list_create(void) {
    algokit_list *list = (algokit_list *)malloc(sizeof(*list));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void algokit_list_destroy(algokit_list *list, algokit_item_destroy_fn destroy) {
    struct algokit_list_node *node;

    if (!list) {
        return;
    }

    node = list->head;
    while (node) {
        struct algokit_list_node *next = node->next;
        if (destroy) {
            destroy(node->item.key, node->item.val);
        }
        free(node);
        node = next;
    }

    free(list);
}

int algokit_list_push_front(algokit_list *list, Item item) {
    struct algokit_list_node *node;

    if (!list) {
        return -1;
    }

    node = (struct algokit_list_node *)malloc(sizeof(*node));
    if (!node) {
        return -1;
    }

    node->item = item;
    node->next = list->head;
    list->head = node;
    if (!list->tail) {
        list->tail = node;
    }
    list->size++;

    return 0;
}

int algokit_list_push_back(algokit_list *list, Item item) {
    struct algokit_list_node *node;

    if (!list) {
        return -1;
    }

    node = (struct algokit_list_node *)malloc(sizeof(*node));
    if (!node) {
        return -1;
    }

    node->item = item;
    node->next = NULL;

    if (!list->tail) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;
    return 0;
}

int algokit_list_pop_front(algokit_list *list, Item *out) {
    struct algokit_list_node *node;

    if (!list || !out || !list->head) {
        return -1;
    }

    node = list->head;
    *out = node->item;
    list->head = node->next;
    if (!list->head) {
        list->tail = NULL;
    }
    list->size--;
    free(node);

    return 0;
}

int algokit_list_peek_front(const algokit_list *list, Item *out) {
    if (!list || !out || !list->head) {
        return -1;
    }

    *out = list->head->item;
    return 0;
}

size_t algokit_list_size(const algokit_list *list) {
    if (!list) {
        return 0;
    }

    return list->size;
}

int algokit_list_is_empty(const algokit_list *list) {
    return !list || list->size == 0;
}
