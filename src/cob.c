#include <malloc.h>
#include <string.h>
#include <math.h>
#include "cob.h"

COB* cobtreeCreate(Serializer* keySerializer, Serializer* valueSerializer, int (*keyCompare) (const void*, const void*), void* items) {
    COB* cobtree = malloc(sizeor(*COB));
    if (cobtree == NULL) return NULL;

    //is the list of items sorted?
    Btree* btree = btreeCreate(keySerializer, valSerializer, keyCompare);
    if (btree == NULL) return NULL;

    double logN = log(size)

    int numMidLevelGroups = ceil(numBottomLevelGroups/logN);
    int numBottomLevelGroups = ceil(size/logN);

    cobtree->topLevel = btree;
    cobtree->midLevel = NULL; //pma?
    cobtree->bottomLevel = NULL; //pma
    cobtree->size = size;



    return cobtree;

}
