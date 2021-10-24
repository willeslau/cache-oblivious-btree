#include <stdio.h>
#include "src/btree.h"
#include <time.h>

int intKeyCompare(const void* key1, const void* key2) {
    int v = *((int*)key1) - *((int*)key2);
    return v;
}

void printIntKeyValue(void* key, void* val) {
    if (key == NULL) printf("NULL\n");
    else printf("(%d, %d) -> ", *(int*)key, *(int*)val);
}

void sanityCheck() {
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
}

typedef struct Key {
    int key;
} Key;

int keyCompare(const void* key1, const void* key2) {
    return ((Key*)key1)->key - ((Key*)key2)->key;
}

void speed() {
    unsigned int size = 10000;
    void** keys = malloc(sizeof(void*) * size);
    for (int i = 0; i < size; i++) {
        Key* key = malloc(sizeof(Key));
        key->key = i;
        keys[i] = (void*) key;
    }

    Btree* btree = btreeCreate(keyCompare);
    clock_t start_time = clock();
    for (int i = 0; i < size; i++) {
        insert(btree, keys[i], keys[i]);
    }
    clock_t elapsed_time = clock() - start_time;
    printf("Done %d inserts in %f seconds. ", size, (double)elapsed_time / CLOCKS_PER_SEC);
    printf("Per loop time: %f ms\n", 1000 * (double)elapsed_time / size / CLOCKS_PER_SEC);
}

int main() {
//    sanityCheck();
    speed();
    return 0;
}
