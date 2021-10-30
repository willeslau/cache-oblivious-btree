/*
 * The Packed Memory Array implementation mentioned in the paper:
 * https://erikdemaine.org/papers/CacheObliviousBTrees_SICOMP/paper.pdf
 *
 * This is divided into Segment and PMA separately. Each segment is basically
 * the O(logN) items mentioned in the paper. PMA is made of joining the segments
 * into a bigger array.
 */

#include <stdbool.h>

/* The status codes */
#define PMA_OK 0
#define PMA_ERROR 1
#define MAX_SEGMENT_SIZE 32

// We can refer to https://github.com/lodborg/cache-oblivious-btree/blob/master/src/main/java/com/lodborg/btree/Segment.java

typedef struct Segment {
    /* Pointer to the array of items */
    void** items;
    /* The size of each item, for malloc */
    long itemSize;
    /* The size of the segment, refers to O(logN) in the paper.
     * This number is not known if N is not known. Also make sure
     * this number is of 2^n for some bit-wise optimization.
     */
    unsigned int capacity;
    /* The current size of the Segment. */
    unsigned int size;
    /* Bitmap indicate if items[i] is set. Int is enough due to MAX_SEGMENT_SIZE */
    bool bitmap[MAX_SEGMENT_SIZE];
} Segment;

/* Creates an empty segment */
Segment* emptySegment(long itemSize, int capacity);

/* Insert into the next free slot starting from the idx.
 * Performs memcpy? Returns status code.
 */
int insert(Segment* segment, unsigned int idx, const void* item);

/* Set the idx to the item, performs memcpy? Returns status code */
int set(Segment* segment, unsigned int idx, const void* item);

/* Get the next empty location to the right of idx. -1 if none. */
int getNextFreeRight(Segment* segment, unsigned int idx);

/* Get the next empty location to the left of idx. -1 if none. */
int getNextFreeLeft(Segment* segment, unsigned int idx);

/* Serialize the segment */
char* serialize(Segment* segment);

int segmentRemove(Segment* segment, unsigned int idx);


// ================= PMA related =================
// Can refer to: https://github.com/lodborg/cache-oblivious-btree/blob/master/src/main/java/com/lodborg/btree/PackedMemoryArray.java
typedef struct PackedMemoryArray {
    /* The size of the segment, refers to O(logN) in the paper.
     * This number is not known if N is not known. Also make sure
     * this number is of 2^n for some bit-wise optimization.
     */
    int segmentSize;
    int segmentCount;
    int height;
} PMA;

/* Get the pointer to the data in the PMA at idx */
void* segmentGet(PMA* pma, unsigned int idx);
/* Set the value of the data in the PMA at idx */
void segmentSet(PMA* pma, unsigned int idx, void* item);
/* Insert the value of the data in the PMA from idx */
void segmentInsert(PMA* pma, unsigned int idx, void* item);