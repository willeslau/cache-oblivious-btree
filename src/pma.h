/*
 * The Packed Memory Array implementation mentioned in the paper:
 * https://erikdemaine.org/papers/CacheObliviousBTrees_SICOMP/paper.pdf
 *
 * This is divided into Segment and PMA separately. Each segment is basically
 * the O(logN) items mentioned in the paper. PMA is made of joining the segments
 * into a bigger array.
 */

#include <stdbool.h>
#include <sys/types.h>

/* The status codes */
#define PMA_OK 0
#define PMA_ERROR 1
#define MAX_SEGMENT_SIZE 32

// We can refer to https://github.com/lodborg/cache-oblivious-btree/blob/master/src/main/java/com/lodborg/btree/Segment.java

typedef struct Segment {
    /* Pointer to the array of items */
    unsigned char* data;
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
 * Performs memcpy. Returns status code.
 */
int segmentInsert(Segment* segment, void* key, void* val);

/* Find the value by the key */
void* segmentFind(Segment* segment, void* key);

/* Serialize the segment */
char* serialize(Segment* segment);

int segmentRemove(Segment* segment, unsigned int idx);

/* Get the next empty location to the right of idx. -1 if none. */
static int getNextFreeRight(Segment* segment, unsigned int idx);

/* Get the next empty location to the left of idx. -1 if none. */
static int getNextFreeLeft(Segment* segment, unsigned int idx);


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

/* Create a PMA */
PMA* pmaCreate(long itemSize, int capacity);

/* Insert the value into the PMA */
int pmaInsert(PMA* pma, void* key, void* val);

/* Find the value in the PMA by the key */
void* pmaFind(PMA* pma, void* key);
