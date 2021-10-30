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

typedef struct Key {
    int key;
} Key;

int keyCompare(const void* key1, const void* key2) {
    return ((Key*)key1)->key - ((Key*)key2)->key;
}

char* serializeKey(void* key) {
    char* data = malloc(4);
    Key* k = (Key*)key;
    serializeUInt(data, k->key, 0);
    return data;
}

void sanityCheck() {
    Serializer *serializer = malloc(sizeof(*serializer));
    serializer->itemSize = 4;
    serializer->process = serializeKey;

    Btree* btree = btreeCreate(serializer, serializer, intKeyCompare);

    unsigned int size = 200;
    void** keys = malloc(sizeof(void*) * size);
    for (int i = 0; i < size; i++) {
        Key* key = malloc(sizeof(Key));
        key->key = i;
        keys[i] = (void*) key;
    }
    for (int i = 0; i < size; i++) {
        insert(btree, keys[i], keys[i]);
    }
    printf("tree height: %d\n", btree->height);
}

void speed() {

    Serializer *serializer = malloc(sizeof(*serializer));
    serializer->itemSize = 4;
    serializer->process = serializeKey;

    unsigned int size = 10000;
    void** keys = malloc(sizeof(void*) * size);
    for (int i = 0; i < size; i++) {
        Key* key = malloc(sizeof(Key));
        key->key = i;
        keys[i] = (void*) key;
    }

    Btree* btree = btreeCreate(serializer, serializer, keyCompare);
    clock_t start_time = clock();
    for (int i = 0; i < size; i++) {
        insert(btree, keys[i], keys[i]);
    }
    clock_t elapsed_time = clock() - start_time;
    printf("Done %d inserts in %f seconds. ", size, (double)elapsed_time / CLOCKS_PER_SEC);
    printf("Per loop time: %f ns\n", 1000000000 * (double)elapsed_time / size / CLOCKS_PER_SEC);
    btreeMetadata(btree);
}

int main() {
    sanityCheck();
//    speed();
    return 0;
}
