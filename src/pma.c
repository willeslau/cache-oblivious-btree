#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "pma.h"
#include "util.h"

PMA* initPMA(PMA* pma, long itemSize, int capacity, int segmentSize, int (*compare) (const void*, const void*)) {
    pma->itemSize = itemSize;
    pma->capacity = normalize(capacity);
    pma->segmentSize = segmentSize;
    pma->numSegment = ceil((double)pma->capacity / pma->segmentSize);
    pma->size = 0;
    pma->height = floorLg(pma->numSegment) + 1;
    pma->keyCompare = compare;
    pma->data = malloc(itemSize * pma->capacity);
    pma->isOccupied = malloc(sizeof(bool) * pma->capacity);

    return pma;
}

PMA* emptyPMA(long itemSize, int capacity, int segmentSize, int (*compare) (const void*, const void*)) {
    PMA* pma = malloc(sizeof(*pma));
    ensure((pma != NULL), "cannot init pma");

    initPMA(pma, itemSize, capacity, segmentSize, compare);
    ensure((pma->data != NULL), "cannot init pma");

    for (int i = 0; i < pma->capacity; i++) {
        pma->isOccupied[i] = false;
    }

    return pma;
}

/*
 * Array:
 * |    |  2  |  4  |  5  |  7  |
 *
 * key: 6
 *
 * Find nearest smaller
 * idx: 3
 */
bool pmaFindInner(PMA* pma, void* keyOnly, unsigned int* idx) {
    //printf("in findinner\n");
    ensure((pma->size > 0), "invalid size");

    int lo = 0;
    int hi = pma->capacity-1;

    // perform modified binary search
    int mid;
    while (lo <= hi) {
        mid = ((hi - lo) >> 1) + lo;

        // scan left to see until an empty item
        int i = mid;
        while (i >= lo && !pma->isOccupied[i]) i--;
        // all the lower slots are empty
        if (i < lo) lo = mid + 1;
        else {
            // now perform the compares
            int compare = pma->keyCompare(pma->data + pma->itemSize * i, keyOnly);
            if (compare == 0) {
                *idx = i;
                return true;
            }
            else if (compare > 0) { hi = i - 1; }
            else { lo = mid + 1; }
        }
    }
    // not found, now return the next vacant location
    *idx = hi;
    while (*idx >= 0 && !pma->isOccupied[*idx]) (*idx)--;
    return false;
}

int pmaFind(PMA* pma, void* keyOnly) {
    unsigned int idx;
    return pmaFindInner(pma, keyOnly, &idx) ? (int)idx : -1;
}

static void setAtIndex(PMA* pma, unsigned int idx, void* item) {
    //printf("in set at index\n");
    ensure((idx < pma->capacity), "idx overflow");
    memcpy(pma->data + pma->itemSize * idx, item, pma->itemSize);
   //printf("%d: %d\n", idx, pma->isOccupied[idx]);
    pma->isOccupied[idx] = true;
}

static void pmaShiftData(PMA* pma, unsigned int src, unsigned int dst) {
    memcpy(pma->data + pma->itemSize * dst, pma->data + pma->itemSize * src, pma->itemSize);
    pma->isOccupied[dst] = pma->isOccupied[src];
    pma->isOccupied[src] = false;
}

// Caller ensure indexes are valid
static void pmaCopyData(PMA* pma, unsigned int src, unsigned int dst) {
    memcpy(pma->data + pma->itemSize * dst, pma->data + pma->itemSize * src, pma->itemSize);
    pma->isOccupied[dst] = true;
}

static int findFirstVacantAfter(PMA* pma, unsigned int idx) {
    for (unsigned int i = idx; i < pma->capacity; i++) {
        if (pma->isOccupied[i]) { continue; }
        return (int)idx;
    }
    return -1;
}

static int findFirstVacantBefore(PMA* pma, unsigned int idx) {
    for (int i = (int)idx; i >= 0; i--) {
        if (pma->isOccupied[i]) { continue; }
        return i;
    }
    return -1;
}

static void dataLeftShift(PMA* pma, unsigned int start, unsigned int end) {
    ensure((start > 0), "start index too small");
    for (unsigned int i = start-1; i < end; i++) {
        pmaShiftData(pma, (i+1), i);
//        memcpy(pma->data + pma->itemSize * i, pma->data + pma->itemSize * (i+1), pma->itemSize);
    }
}

static void dataRightShift(PMA* pma, unsigned int start, unsigned int end) {
    ensure((end < pma->capacity-1), "end index too large");
    for (unsigned int i = end+1; i > start; i--) {
        pmaShiftData(pma, (i-1), i);
//        memcpy(pma->data + pma->itemSize * i, pma->data + pma->itemSize * (i-1), pma->itemSize);
    }
}

unsigned int pmaSize(PMA* pma) {
    return pma->size;
}

/* Squash the empty slots in the pma to the start of the array, inclusive */
static void pack(PMA* pma, unsigned int start, unsigned int end) {
    ensure((start < end), "invalid pack range");

    unsigned int head = start;
    while (head <= end && pma->isOccupied[head]) head++;
    unsigned int tail = head + 1;
    while (1) {
        while (tail <= end && !pma->isOccupied[tail]) tail++;
        // tail reached end and all slots are empty
        if (tail > end) { break; }
        // TODO: can optimize with batch copy
        pmaCopyData(pma, tail, head);
        pma->isOccupied[tail] = false;
        head++;
        tail++;
    }
}

/* Redistribute the items within start and end EVENLY..., indexes are inclusive */
static void redistribute(PMA* pma, unsigned int start, unsigned int end, unsigned int size) {
    ensure((start < end), "invalid redistribute range");

    unsigned int vacancy = end - start + 1 - size;
    // we are sharing the vacancy to each of the items
    unsigned int gap = vacancy / size;
    // not enough free space, give up
    if (gap == 0) return;
    // with enough gap, let's reshuffle
    unsigned int head = start + size - 1;
    unsigned int tail = end;
    while (head < tail && tail >= start) {
        pmaShiftData(pma, head, tail);
        head--;
        tail -= gap;
    }
}

static void computeCapacity (PMA* pma) {
    pma->segmentSize = (int) ceilLg((unsigned int)pma->size);
    pma->numSegment = (int) hyperCeil(ceilDiv(pma->capacity, pma->segmentSize));
    pma->segmentSize = (int) ceilDiv(pma->size, pma->numSegment);
    pma->capacity = pma->segmentSize * pma->numSegment;
    /* Scale up as much as possible. */
    pma->capacity *= MAX_SPARSENESS;
    pma->numSegment *= MAX_SPARSENESS;
}

static void resize(PMA* pma) {
    pack(pma, 0, pma->capacity);
    computeCapacity(pma);
    pma->height = floorLg(pma->numSegment) + 1;
    pma->hiThreshold = (LEAF_HI_THRESHOLD - ROOT_HI_THRESHOLD) / pma->height;
    pma->loThreshold = (ROOT_LO_THRESHOLD - LEAF_LO_THRESHOLD) / pma->height;
    pma->data = realloc(pma->data, sizeof (pma->itemSize) * pma->capacity);
    pma->isOccupied = realloc(pma->isOccupied, sizeof (bool) * pma->capacity);
    for (unsigned int i = pma->size; i < pma->capacity; i++) pma->isOccupied[i] = false;
    redistribute(pma, 0, pma->capacity, pma->size);
}

/* Re-balance the pma from the index, caller ensure the validity of the idx */
static void rebalance(PMA* pma, int idx) {
    int occupancy = pma->isOccupied[idx] ? 1 : 0;
    int windowStart, windowEnd, windowSize, depth = 0;
    int start = idx - 1, end = idx + 1;
    double hiThreshold, loThreshold, density;

    while (1) {
        windowSize = pma->segmentSize * (1 << depth);
        windowStart = idx / windowSize * windowSize;
        windowEnd = windowStart + windowSize < pma->capacity? windowStart + windowSize - 1: pma->capacity-1;

        /*
         * Array:
         *  |  2  |  4  |  5  |  6  |  7  |
         *     |                 |     |
         * windowStart          idx windowEnd
         */
        // scan to the left
        while (start >= windowStart) {
            if (pma->isOccupied[start]) occupancy++;
            start--;
        }
        // scan to the right
        while (end <= windowEnd) {
            if (pma->isOccupied[end]) occupancy++;
            end++;
        }
        density = (double) occupancy / windowSize;
        hiThreshold = LEAF_HI_THRESHOLD - (depth * pma->hiThreshold);
        loThreshold = LEAF_LO_THRESHOLD + (depth * pma->loThreshold);
        depth++;

        if (depth > pma->height) {
            resize(pma);
            break;
        }
        // array is not packed at all
        if (density <= loThreshold) break;
        if (density < hiThreshold && density > loThreshold) {
            pack(pma, windowStart, windowEnd);
            redistribute(pma, windowStart, windowEnd, occupancy);
            break;
        }
    }
}

void pmaInsertAfter(PMA* pma, void* item, unsigned int idx) {
    ensure((pma->size < pma->capacity), "pma full");
    /*
     * Array:
     * |    |  2  |  4  |  5  |  7  |
     *
     * key: 6
     * idx: 3
     */
    int i = findFirstVacantAfter(pma, idx+1);
    // no vacancy after idx, search from before
    if (i == -1) {
        i = findFirstVacantBefore(pma,idx-1);
        ensure((i >= 0), "inconsistent state");
        // |  2  |  4  |  5  |  6  |  7  |
        dataLeftShift(pma, (unsigned int)i+1, idx);
        setAtIndex(pma, idx, item);
    } else {
        dataRightShift(pma, idx+1, (unsigned int)i-1);
        setAtIndex(pma, idx+1, item);
    }
    pma->size++;

    rebalance(pma, idx);
}

/*
 * Array:
 * |    |  2  |  4  |  5  |  7  |
 *
 * item: 6
 */
int pmaInsert(PMA* pma, void* item) {
    ensure((pma->size < pma->capacity), "pma full");
    if (pma->size == 0) {
        setAtIndex(pma, 0, item);
        pma->size++;
        return PMA_OK;
    }

    unsigned int idx;
    if (pmaFindInner(pma, item, &idx)) setAtIndex(pma, idx, item);
    else pmaInsertAfter(pma, item, idx);
    return PMA_OK;
}