/*
 * The Packed Memory Array implementation mentioned in the paper:
 * https://erikdemaine.org/papers/CacheObliviousBTrees_SICOMP/paper.pdf
 */

#include <stdbool.h>
#include <sys/types.h>

/* The status codes */
#define PMA_OK 0
#define PMA_ERROR 1
#define MAX_SEGMENT_SIZE 32

/* Density threshold */
#define LEAF_HI_THRESHOLD 1.00
#define LEAF_LO_THRESHOLD 0.25
#define ROOT_HI_THRESHOLD 0.75
#define ROOT_LO_THRESHOLD 0.50
static const unsigned int MAX_SPARSENESS = (unsigned int)(1 / LEAF_LO_THRESHOLD);

typedef struct PackedMemoryArray {
    int segmentSize;
    int numSegment;
    /* The capacity of the pma, refers to O(logN) in the paper.
     * This number is not known if N is not known. Also make sure
     * this number is of 2^n for some bit-wise optimization.
     */
    int capacity;
    /* The current size of the pma. */
    unsigned int size;
    /* The current height of the tree. */
    unsigned int height;
    /* The size of each item, for malloc */
    long itemSize;
    /* Item compare function */
    int (*keyCompare) (const void*, const void*);
    /* Pointer to the array of items */
    unsigned char* data;
    /* Bitmap indicate if items[i] is set */
    bool* isOccupied;
    double hiThreshold;
    double loThreshold;
} PMA;

unsigned int pmaSize(PMA* pma);
int pmaInsert(PMA* pma, void* item);
int pmaFind(PMA* pma, void* keyOnly);
void pmaInsertAfter(PMA* pma, void* item, unsigned int idx);
bool pmaFindInner(PMA* pma, void* keyOnly, unsigned int* idx);
PMA* emptyPMA(long itemSize, int capacity, int segmentSize, int (*compare) (const void*, const void*));

/* Get the next empty location to the right of idx. -1 if none. */
static int getNextFreeRight(PMA* pma, unsigned int idx);

/* Get the next empty location to the left of idx. -1 if none. */
static int getNextFreeLeft(PMA* pma, unsigned int idx);
