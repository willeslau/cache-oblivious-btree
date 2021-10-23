#include <stdio.h>
#include "src/btree.h"

int intKeyCompare(const void* key1, const void* key2) {
    int v = *((int*)key1) - *((int*)key2);
    return v;
}

void printIntKeyValue(void* key, void* val) {
    if (key == NULL) printf("NULL\n");
    else printf("(%d, %d) -> ", *(int*)key, *(int*)val);
}

int main() {
    Btree* btree = btreeCreate(intKeyCompare);

    int k1 = 2, v1 = 2;
    insert(btree, &k1, &v1);
    printElements(btree, printIntKeyValue);

    int k2 = 2, v2 = 2;
    insert(btree, &k2, &v2);
    printElements(btree, printIntKeyValue);

    int k3 = 4, v3 = 2;
    insert(btree, &k3, &v3);
    printElements(btree, printIntKeyValue);

    int k4 = 1, v4 = 2;
    insert(btree, &k4, &v4);

    int k5 = 0, v5 = 2;
    insert(btree, &k5, &v5);
    printElements(btree, printIntKeyValue);
    printf("tree height: %d\n", btree->height);

    int k6 = 5, v6 = 2;
    insert(btree, &k6, &v6);

    int k7 = 6, v7 = 2;
    insert(btree, &k7, &v7);

    int k8 = 7, v8 = 2;
    insert(btree, &k8, &v8);

    printElements(btree, printIntKeyValue);
    printf("tree height: %d\n", btree->height);

    return 0;
}
