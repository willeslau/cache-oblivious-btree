#include <malloc.h>
#include <memory.h>
#include "pma.h"
#include "serialization.h"

unsigned int normalize(int num) {
    unsigned int i = 1;
    while (i < num) {
        i  = i << 1;
    }
    return i;
}

Segment* emptySegment(long itemSize, int capacity, int (*compare) (const void*, const void*)) {
    Segment* segment = malloc(sizeof(*segment));
    ensure((segment != NULL), "cannot init segment");

    segment->itemSize = itemSize;
    segment->capacity = normalize(capacity);
    segment->size = 0;
    segment->keyCompare = compare;
    segment->data = malloc(itemSize * segment->capacity);
    ensure((segment->data != NULL), "cannot init segment");

    return segment;
}

bool segmentFindInner(Segment* segment, void* keyOnly, unsigned int* idx) {
    unsigned int lo = 0;
    unsigned int hi = segment->capacity-1;

    // perform modified binary search
    unsigned int mid;
    while (lo <= hi) {
        mid = ((hi - lo) >> 2) + lo;

        // scan left to see until an empty item
        unsigned int i = mid;
        while (i >= lo && !segment->bitmap[i]) i--;
        // all the lower slots are empty
        if (i < lo) lo = mid + 1;
        else {
            // now perform the compares
            int compare = segment->keyCompare(segment->data + segment->itemSize * i, keyOnly);
            if (compare == 0) {
                *idx = mid;
                return true;
            }
            else if (compare < 0) { hi = mid - 1; }
            else { lo = mid + 1; }
        }
    }
    // not found, now return the next vacant location
    *idx = hi;
    while (*idx >= 0 && !segment->bitmap[*idx]) (*idx)--;
    return false;
}

int segmentFind(Segment* segment, void* keyOnly) {
    unsigned int idx;
    return segmentFindInner(segment, keyOnly, &idx) ? (int)idx : -1;
}

static void setAtIndex(Segment* segment, unsigned int idx, void* item) {
    ensure((idx < segment->capacity), "idx overflow");
    memcpy(segment->data + segment->itemSize * idx, item, segment->itemSize);
}

static int findFirstVacantAfter(Segment* segment, unsigned int idx) {
    for (unsigned int i = idx; i < segment->capacity; i++) {
        if (segment->bitmap[i]) { continue; }
        return (int)idx;
    }
    return -1;
}

static int findFirstVacantBefore(Segment* segment, unsigned int idx) {
    for (int i = (int)idx; i >= 0; i--) {
        if (segment->bitmap[i]) { continue; }
        return i;
    }
    return -1;
}

static void dataLeftShift(Segment* segment, unsigned int start, unsigned int end) {
    ensure((start > 0), "start index too small");
    for (unsigned int i = start-1; i < end; i++) {
        memcpy(segment->data + segment->itemSize * i, segment->data + segment->itemSize * (i+1), segment->itemSize);
    }
}

static void dataRightShift(Segment* segment, unsigned int start, unsigned int end) {
    ensure((end < segment->capacity-1), "end index too large");
    for (unsigned int i = end+1; i > start; i--) {
        memcpy(segment->data + segment->itemSize * i, segment->data + segment->itemSize * (i-1), segment->itemSize);
    }
}

void segmentInsertAfter(Segment* segment, void* item, unsigned int idx) {
    ensure((segment->size < segment->capacity), "segment full");
    int i = findFirstVacantAfter(segment, idx+1);
    // no vacancy after idx, search from before
    if (i == -1) {
        i = findFirstVacantBefore(segment,idx-1);
        ensure((i >= 0), "inconsistent state");
        dataLeftShift(segment, (unsigned int)i+1, idx);
        setAtIndex(segment, idx, item);
        segment->bitmap[idx] = true;
    } else {
        dataRightShift(segment, idx+1, (unsigned int)i-1);
        setAtIndex(segment, idx+1, item);
        segment->bitmap[idx+1] = true;
    }
    segment->size++;
}

int segmentInsert(Segment* segment, void* item) {
    ensure((segment->size < segment->capacity), "segment full");
    unsigned int idx;
    if (segmentFindInner(segment, item, &idx)) setAtIndex(segment, idx, item);
    else segmentInsertAfter(segment, item, idx);
    return PMA_OK;
}
