#include <algokit/item.h>

/*
 * Context: minimal program that links against libalgokit.a without using
 * any specific data structure. This validates the build and link steps.
 */
int main(void) {
    Item item;

    /* Initialize the generic Item to demonstrate the public header. */
    item.key = NULL;
    item.val = NULL;

    return 0;
}
