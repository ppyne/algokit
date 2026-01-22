#include "algokit/queue.h"

#include <stdlib.h>

struct algokit_queue_node {
    Item item;
    struct algokit_queue_node *next;
};

struct algokit_queue {
    struct algokit_queue_node *head;
    struct algokit_queue_node *tail;
    size_t size;
};

algokit_queue *algokit_queue_create(void) {
    algokit_queue *queue = (algokit_queue *)malloc(sizeof(*queue));
    if (!queue) {
        return NULL;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

void algokit_queue_destroy(algokit_queue *queue, algokit_item_destroy_fn destroy) {
    struct algokit_queue_node *node;

    if (!queue) {
        return;
    }

    node = queue->head;
    while (node) {
        struct algokit_queue_node *next = node->next;
        if (destroy) {
            destroy(node->item.key, node->item.val);
        }
        free(node);
        node = next;
    }

    free(queue);
}

int algokit_queue_enqueue(algokit_queue *queue, Item item) {
    struct algokit_queue_node *node;

    if (!queue) {
        return -1;
    }

    node = (struct algokit_queue_node *)malloc(sizeof(*node));
    if (!node) {
        return -1;
    }

    node->item = item;
    node->next = NULL;

    if (!queue->tail) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }

    queue->size++;
    return 0;
}

int algokit_queue_dequeue(algokit_queue *queue, Item *out) {
    struct algokit_queue_node *node;

    if (!queue || !out || !queue->head) {
        return -1;
    }

    node = queue->head;
    *out = node->item;
    queue->head = node->next;
    if (!queue->head) {
        queue->tail = NULL;
    }

    queue->size--;
    free(node);
    return 0;
}

int algokit_queue_peek(const algokit_queue *queue, Item *out) {
    if (!queue || !out || !queue->head) {
        return -1;
    }

    *out = queue->head->item;
    return 0;
}

size_t algokit_queue_size(const algokit_queue *queue) {
    if (!queue) {
        return 0;
    }

    return queue->size;
}

int algokit_queue_is_empty(const algokit_queue *queue) {
    return !queue || queue->size == 0;
}
