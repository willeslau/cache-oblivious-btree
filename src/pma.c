#include <malloc.h>
#include "pma.h"
#include "serialization.h"

unsigned int normalize(int num) {
    unsigned int i = 1;
    while (i < num) {
        i  = i << 1;
    }
    return i;
}

Segment* emptySegment(long itemSize, int capacity) {
    Segment* segment = malloc(sizeof(*segment));
    ensure((segment != NULL), "cannot init segment");

    segment->itemSize = itemSize;
    segment->capacity = normalize(capacity);
    segment->size = 0;
    segment->items = malloc(itemSize * segment->capacity);
    ensure((segment->items != NULL), "cannot init segment");

    return segment;
}

int insert(Segment* segment, unsigned int idx, const void* item) {
    if (segment->bitmap)
}

