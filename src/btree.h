/*
 * This is an implementation of B+Tree. It includes the macro, function
 * and struct declarations needed for subsequent implementations.
 */

#include <stdlib.h>
#include <stdbool.h>

#define MAX_CHILDREN 32
#define MIN_CHILDREN 16

#define ensure(expr, msg) if (!expr) { perror(msg); exit(EXIT_FAILURE); }

/*
 * The B+tree internal data node
 */
typedef struct BtreeNode {
    short size;
    void** keys;
    struct BtreeNode* parent;
    /* This should be an array of nodes */
    struct BtreeNode* links;
    /* For leaf nodes */
    bool is_leaf;
    void** items;
    struct BtreeNode* prev;
    struct BtreeNode* next;
} BtreeNode;

/*
 * The B+tree data structure
 */
typedef struct Btree {
    BtreeNode* root;
    /* The current size of the Btree */
    unsigned int size;
    /* The current height of the Btree */
    unsigned int height;
    /* The internal function to compare different keys */
    int (*keyCompare) (const void*, const void*);
} Btree;

/*
 * Create a btree, returns the pointer.
 * We pass in the key comparison function for future key compares
 */
Btree* btreeCreate(int (*keyCompare) (const void*, const void*));
void insert(Btree* btree, void* key, void* value);
int btreeFree(Btree* btree);
