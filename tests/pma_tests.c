//#include <malloc.h>
//#include <time.h>
//#include "../src/pma.h"
//
//typedef struct KeyVal {
//    int key;
//    int val;
//} KeyVal;
//
//int keyCompare(const void* key1, const void* key2) {
//    int k1 = ((KeyVal*)key1)->key;
//    int k2 = ((KeyVal*)key2)->key;
//    return k1 - k2;
//}
//
//int main() {
//    int capacity = 16;
//    PMA* pma = emptyPMA(sizeof(KeyVal), capacity, 16, keyCompare);
//
//    int numItems = 10000;
//    KeyVal** items = malloc(sizeof(void*) * numItems);
//    KeyVal* item;
//    for (int i = 0; i < numItems; i++) {
//        item = malloc(sizeof(KeyVal));
//        item->key = i;
//        item->val = i;
//        items[i] = item;
//    }
//
//    clock_t start_time = clock();
//    for (int i = 0; i < numItems; i++) {
//        pmaInsert(pma, items[i]);
//    }
//    clock_t elapsed_time = clock() - start_time;
//    printf("Done %d inserts in %f seconds. ", numItems, (double)elapsed_time / CLOCKS_PER_SEC);
//    printf("Per loop time: %f ns\n", 1000000000 * (double)elapsed_time / numItems / CLOCKS_PER_SEC);
//
//}