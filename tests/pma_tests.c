#include <malloc.h>
#include "../src/pma.h"

typedef struct KeyVal {
    int key;
    int val;
} KeyVal;

int keyCompare(const void* key1, const void* key2) {
    int k1 = ((KeyVal*)key1)->key;
    int k2 = ((KeyVal*)key2)->key;
    return k1 - k2;
}

int main() {
    int capacity = 16;
    PMA* pma = emptyPMA(sizeof(KeyVal), capacity, 16, keyCompare);

    int numItems = 15;
    KeyVal** items = malloc(sizeof(void*) * numItems);
    KeyVal* item;
    for (int i = 0; i < numItems; i++) {
        item = malloc(sizeof(KeyVal));
        item->key = i;
        item->val = i;
        items[i] = item;
    }

    for (int i = 0; i < numItems; i++) {
        pmaInsert(pma, items[i]);
    }
}