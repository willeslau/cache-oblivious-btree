/*
 * The cache oblivious btree implementation
 */
#include "util.h"
#include "btree.h"
#include "pma.h"
#include <stdbool.h>

typedef struct CacheObliviousBTree {
    Btree* topLevel;
    PMA* midLevel;
    PMA* bottomLevel;
    int size;
    int (*keyCompare) (const void*, const void*);
} COB;

COB* cobtreeCreate(Serializer* keySerializer, Serializer* valueSerializer, int (*keyCompare) (const void*, const void*));
PMA* getRepElements(void* items, int numGroups);
void cobInsert(COB* cob, void* key, void* value);