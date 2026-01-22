#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "algokit/list.h"

int main(void) {
    enum { kCount = 1000 };
    int values[kCount];
    size_t i;
    Item item;
    algokit_list *list = algokit_list_create();

    printf("list_test: create\n");
    assert(list != NULL);
    assert(algokit_list_is_empty(list));
    assert(algokit_list_size(list) == 0);

    printf("list_test: push_back %d items\n", kCount);
    for (i = 0; i < kCount; i++) {
        values[i] = (int)i;
        item.key = &values[i];
        item.val = NULL;
        assert(algokit_list_push_back(list, item) == 0);
    }

    assert(algokit_list_size(list) == kCount);

    printf("list_test: pop_front %d items\n", kCount);
    for (i = 0; i < kCount; i++) {
        assert(algokit_list_pop_front(list, &item) == 0);
        assert(item.key == &values[i]);
    }

    assert(algokit_list_is_empty(list));
    assert(algokit_list_size(list) == 0);

    printf("list_test: destroy\n");
    algokit_list_destroy(list, NULL);
    printf("list_test: ok\n");
    return 0;
}
