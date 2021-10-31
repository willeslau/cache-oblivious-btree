#include <malloc.h>
#include <memory.h>
#include "pma.h"
#include "util.h"

PMA* emptyPMA(long itemSize, int capacity, int (*compare) (const void*, const void*)) {
    PMA* pma = malloc(sizeof(*pma));
    ensure((pma != NULL), "cannot init pma");

    pma->itemSize = itemSize;
    pma->capacity = normalize(capacity);
    pma->size = 0;
    pma->keyCompare = compare;
    pma->data = malloc(itemSize * pma->capacity);
    ensure((pma->data != NULL), "cannot init pma");

    return pma;
}

bool pmaFindInner(PMA* pma, void* keyOnly, unsigned int* idx) {
    unsigned int lo = 0;
    unsigned int hi = pma->capacity-1;

    // perform modified binary search
    unsigned int mid;
    while (lo <= hi) {
        mid = ((hi - lo) >> 2) + lo;

        // scan left to see until an empty item
        unsigned int i = mid;
        while (i >= lo && !pma->isOccupied[i]) i--;
        // all the lower slots are empty
        if (i < lo) lo = mid + 1;
        else {
            // now perform the compares
            int compare = pma->keyCompare(pma->data + pma->itemSize * i, keyOnly);
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
    while (*idx >= 0 && !pma->isOccupied[*idx]) (*idx)--;
    return false;
}

int pmaFind(PMA* pma, void* keyOnly) {
    unsigned int idx;
    return pmaFindInner(pma, keyOnly, &idx) ? (int)idx : -1;
}

static void setAtIndex(PMA* pma, unsigned int idx, void* item) {
    ensure((idx < pma->capacity), "idx overflow");
    memcpy(pma->data + pma->itemSize * idx, item, pma->itemSize);
    pma->isOccupied[idx] = true;
}

static void pmaShiftData(PMA* pma, unsigned int src, unsigned int dst) {
    memcpy(pma->data + pma->itemSize * dst, pma->data + pma->itemSize * src, pma->itemSize);
    pma->isOccupied[dst] = true;
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
        memcpy(pma->data + pma->itemSize * i, pma->data + pma->itemSize * (i+1), pma->itemSize);
    }
}

static void dataRightShift(PMA* pma, unsigned int start, unsigned int end) {
    ensure((end < pma->capacity-1), "end index too large");
    for (unsigned int i = end+1; i > start; i--) {
        memcpy(pma->data + pma->itemSize * i, pma->data + pma->itemSize * (i-1), pma->itemSize);
    }
}

void pmaInsertAfter(PMA* pma, void* item, unsigned int idx) {
    ensure((pma->size < pma->capacity), "pma full");
    int i = findFirstVacantAfter(pma, idx+1);
    // no vacancy after idx, search from before
    if (i == -1) {
        i = findFirstVacantBefore(pma,idx-1);
        ensure((i >= 0), "inconsistent state");
        dataLeftShift(pma, (unsigned int)i+1, idx);
        setAtIndex(pma, idx, item);
        pma->isOccupied[idx] = true;
    } else {
        dataRightShift(pma, idx+1, (unsigned int)i-1);
        setAtIndex(pma, idx+1, item);
        pma->isOccupied[idx + 1] = true;
    }
    pma->size++;
}

int pmaInsert(PMA* pma, void* item) {
    ensure((pma->size < pma->capacity), "pma full");
    unsigned int idx;
    if (pmaFindInner(pma, item, &idx)) setAtIndex(pma, idx, item);
    else pmaInsertAfter(pma, item, idx);
    return PMA_OK;
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
    while (head < tail) {
        pmaShiftData(pma, head, tail);
        head--;
        tail -= gap;
    }
}

static void computeCapacity (PMA* pma) {
    pma->segmentSize = mostSigBit(pma->capacity-1);
    pma->numSegment = normalizeUint(ceilDivision(pma->capacity, pma->segmentSize));
    /* Update the segment size accordingly. */
    pma->segmentSize = ceilDivision(pma->size, pma->numSegment);
    pma->capacity = pma->segmentSize * pma->numSegment;
    /* Scale up as much as possible. */
    pma->capacity *= MAX_SPARSENESS;
    pma->segmentSize *= MAX_SPARSENESS;
//    assert (pma->m <= MAX_SIZE);
//    assert (pma->m > pma->n);
}

static void resize(PMA* pma) {
    pack (pma, 0, pma->capacity);
    computeCapacity(pma);
    pma->height = mostSigBit(pma->numSegment);
    pma->hiThreshold = (LEAF_HI_THRESHOLD - ROOT_HI_THRESHOLD) / pma->height;
    pma->loThreshold = (ROOT_LO_THRESHOLD - LEAF_LO_THRESHOLD) / pma->height;
    pma->data = realloc (pma->data, sizeof (pma->itemSize) * pma->capacity);
    for (unsigned int i = pma->size; i < pma->capacity; i++) pma->isOccupied[i] = false;
    redistribute(pma, 0, pma->capacity, pma->size);
}

/* Re-balance the pma from the index, caller ensure the validity of the idx */
static void rebalance(PMA* pma, unsigned int idx) {
    unsigned int occupancy = pma->isOccupied[idx] ? 1 : 0;
    unsigned int windowStart, windowEnd, windowSize, depth = 0;
    unsigned int start = idx - 1, end = idx + 1;
    double hiThreshold, loThreshold, density;
    while (1) {
        windowSize = pma->segmentSize * (depth << 1);
        windowStart = idx / windowSize * windowSize;
        windowEnd = windowStart + windowSize;
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

        if (depth >= pma->height) {
            resize(pma);
            break;
        }
        if (density <= hiThreshold && density >= loThreshold) {
            pack(pma, windowStart, windowEnd);
            redistribute(pma, windowStart, windowEnd, occupancy);
            break;
        }
    }
}
