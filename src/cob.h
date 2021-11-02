/*
 * The cache oblivious btree implementation
 */
#include "util.h"
#include "btree.h"
#include "pma.h"
#include <stdbool.h>

typedef struct CacheObliviousBTree {
    btree* topLevel;
    pma* middleLevel
    pma* bottomLevel;
} COB;

COB* cobtreeCreate(Serializer* keySerializer, Serializer* valueSerializer, int (*keyCompare) (const void*, const void*), void* items, int size);
pma* getRepElements(void* items, int numGroups);
void insert(COB* cob, void* key, void* value);