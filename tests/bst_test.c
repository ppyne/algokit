#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "algokit/bst.h"

static int int_key_cmp(const void *a, const void *b) {
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

struct collect_ctx {
    int *out;
    size_t count;
};

static void collect_keys(const Item *item, void *ctx) {
    struct collect_ctx *c = (struct collect_ctx *)ctx;
    c->out[c->count++] = *(int *)item->key;
}

static int is_sorted(const int *values, size_t n) {
    size_t i;
    for (i = 1; i < n; i++) {
        if (values[i - 1] > values[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    algokit_bst *tree = algokit_bst_create(int_key_cmp);
    int keys[] = {5, 2, 8, 1, 3};
    Item item;
    size_t i;
    int found;
    int removed_key;
    int ordered[5];
    struct collect_ctx ctx;

    printf("bst_test: create\n");
    assert(tree != NULL);

    printf("bst_test: insert\n");
    for (i = 0; i < 5; i++) {
        item.key = &keys[i];
        item.val = NULL;
        assert(algokit_bst_insert(tree, item) == 0);
    }

    printf("bst_test: search\n");
    found = 3;
    assert(algokit_bst_search(tree, &found, &item) == 0);
    assert(*(int *)item.key == 3);

    printf("bst_test: inorder\n");
    ctx.out = ordered;
    ctx.count = 0;
    assert(algokit_bst_traverse_inorder(tree, collect_keys, &ctx) == 0);
    assert(ctx.count == 5);
    assert(is_sorted(ordered, ctx.count));

    printf("bst_test: remove\n");
    removed_key = 2;
    assert(algokit_bst_remove(tree, &removed_key, &item) == 0);
    assert(*(int *)item.key == 2);

    printf("bst_test: search missing\n");
    assert(algokit_bst_search(tree, &removed_key, &item) != 0);

    printf("bst_test: destroy\n");
    algokit_bst_destroy(tree, NULL);
    printf("bst_test: ok\n");
    return 0;
}
