#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cob.h"

COB* cobtreeCreate(Serializer* keySerializer, Serializer* valueSerializer, int (*keyCompare) (const void*, const void*), int segmentSize, int size) {
    COB* cob = malloc(sizeof(*cob));
    if (cob == NULL) return NULL;

    //is the list of items sorted?
    Btree* btree = btreeCreate(keySerializer, valueSerializer, keyCompare);
    if (btree == NULL) return NULL;

    cob->topLevel = btree;
    cob->midLevel = emptyPMA(keySerializer->itemSize, size, segmentSize, keyCompare); //pma?
    cob->bottomLevel = emptyPMA(keySerializer->itemSize, size, segmentSize, keyCompare); //pma
    cob->size = 0;

    return cob;
}

void cobInsert(COB* cob, void* key, void* value) {
    int originalSegments = cob->bottomLevel->numSegment;

    //printf("original: %d\n", originalSegments);
    if (pmaInsert(cob->bottomLevel, key) != PMA_OK) {
        exit(1);
    }

    //if we need to split into more groups, we need to insert into middle/top layers too
    if (cob->bottomLevel->numSegment > originalSegments) {
        for (int i = 0; i < cob->bottomLevel->numSegment; i++) {
            void* repItem = cob->bottomLevel->data + i*cob->bottomLevel->itemSize*cob->bottomLevel->segmentSize;            
            pmaInsert(cob->midLevel, repItem);


            if (i%cob->midLevel->segmentSize == 0) {
                insert(cob->topLevel, repItem, repItem);
            }
        }
    }
    ++cob->size;
}