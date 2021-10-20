/*
 * This is an implementation of B+Tree. It includes the macro, function
 * and struct declarations needed for subsequent implementations.
 */

#define MAX_CHILDREN 32;
#define MIN_CHILDREN 16;

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
    int (*keyCompare) (const void* key1, const void* key2);
} Btree;

/*
 * The B+tree internal data node
 */
typedef struct BtreeNode {
    short size;
    bool leaf;
    BtreeNode links[];
    void* items[];
} BtreeNode;

/*
 * Create a btree, returns the pointer. We pass in the key comparison function for future key compares.
 */
Btree* btreeCreate(int (*keyCompare) (const void* key1, const void* key2));
Btree* insert(Btree* btree, const void* key, const void* value);