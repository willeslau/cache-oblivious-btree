#include <stdio.h>
#include "src/btree.h"

int intKeyCompare(const void* key1, const void* key2) {
    return (int*)key1 > (int*) key2;
}

int main() {
    Btree* btree = btreeCreate(intKeyCompare);
    insert(btree, (void *) 2, (void *) 3);
    insert(btree, (void *) 2, (void *) 4);
    insert(btree, (void *) 3, (void *) 5);
    return 0;
}
