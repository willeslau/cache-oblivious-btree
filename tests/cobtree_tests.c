// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include "../src/cob.h"
// #include <time.h>

// int cobIntKeyCompare(const void* key1, const void* key2) {
//     int v = *((int*)key1) - *((int*)key2);
//     return v;
// }

// void cobPrintIntKeyValue(void* key, void* val) {
//     if (key == NULL) printf("NULL\n");
//     else printf("(%d, %d) -> ", *(int*)key, *(int*)val);
// }

// typedef struct Key {
//     int key;
// } Key;

// int cobKeyCompare(const void* key1, const void* key2) {
//     return ((Key*)key1)->key - ((Key*)key2)->key;
// }

// char* cobSerializeKey(void* key) {
//     char* data = malloc(4);
//     Key* k = (Key*)key;
//     serializeUInt(data, k->key, 0);
//     return data;
// }

// void cobSanityCheck() {
//     Serializer *serializer = malloc(sizeof(*serializer));
//     serializer->itemSize = 4;
//     serializer->process = cobSerializeKey;

//     unsigned int size = 200;
//     unsigned int segmentSize = (int)log2(size);

//     //printf("%d, %d\n", size, segmentSize);

//     COB* cobtree = cobtreeCreate(serializer, serializer, cobIntKeyCompare, segmentSize);


//     void** keys = malloc(sizeof(void*) * size);
//     for (int i = 0; i < size; i++) {
//         Key* key = malloc(sizeof(Key));
//         key->key = i;
//         keys[i] = (void*) key;
//     }
//     for (int i = 0; i < size; i++) {
//         //printf("insert\n");
//         cobInsert(cobtree, keys[i], keys[i]);
//     }
// }

// void cobSpeed() {

//     Serializer *serializer = malloc(sizeof(*serializer));
//     serializer->itemSize = 4;
//     serializer->process = cobSerializeKey;

//     unsigned int size = 5000;
//     unsigned int segmentSize = (int)log2(size);
//     void** keys = malloc(sizeof(void*) * size);

//     //printf("%d, %d\n", size, segmentSize);

//     for (int i = 0; i < size; i++) {
//         Key* key = malloc(sizeof(Key));
//         key->key = i;
//         keys[i] = (void*) key;
//     }

//     COB* cobtree = cobtreeCreate(serializer, serializer, cobKeyCompare, segmentSize);
//     clock_t start_time = clock();
//     for (int i = 0; i < size; i++) {
//         //printf("insert\n");
//         cobInsert(cobtree, keys[i], keys[i]);
//     }
//     clock_t elapsed_time = clock() - start_time;
//     printf("Done %d inserts in %f seconds. ", size, (double)elapsed_time / CLOCKS_PER_SEC);
//     printf("Per loop time: %f ns\n", 1000000000 * (double)elapsed_time / size / CLOCKS_PER_SEC);
// }

// int main() {
//     //cobSanityCheck();
//     cobSpeed();
//     return 0;
// }
