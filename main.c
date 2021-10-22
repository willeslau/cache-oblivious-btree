#include "src/btree.h"

int intKeyCompare(const void* key1, const void* key2) {
    int v = *((int*)key1) - *((int*)key2);
    return v;
}

int main() {
    Btree* btree = btreeCreate(intKeyCompare);

    int k1 = 2, v1 = 2;
    insert(btree, &k1, &v1);

    int k2 = 2, v2 = 2;
    insert(btree, &k2, &v2);

    int k3 = 4, v3 = 2;
    insert(btree, &k3, &v3);

    int k4 = 1, v4 = 2;
    insert(btree, &k4, &v4);

    return 0;
}
