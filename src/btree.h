/*
 * This is an implementation of B+Tree. It includes the macro, function
 * and struct declarations needed for subsequent implementations.
 */


#include <stdbool.h>
#include "hashmap.h"
#include "util.h"

#define MAX_CHILDREN 32
#define MIN_CHILDREN 0

#pragma once

typedef struct ScanResult {
    void* keys;
    void* vals;
    unsigned int size;
} ScanResult;

typedef struct SerializationMeta {
    unsigned int size;
    const unsigned char* data;
} SerializationMeta;

/*
 * The B+tree internal data node
 */
typedef struct BtreeNode {
    unsigned int id;
    unsigned int size;
    void** keys;
    struct BtreeNode** links;
    bool is_leaf;
    void** items;
    struct BtreeNode* next;
} BtreeNode;

/*
 * The B+tree data structure
 */
typedef struct Btree {
    BtreeNode* root;
    DataItem** hashArray;
    unsigned int nextId;
    /* The current size of the Btree */
    unsigned int size;
    /* The current height of the Btree */
    unsigned int height;
    /* The internal function to compare different keys */
    int (*keyCompare) (const void*, const void*);
    Serializer* keySerializer;
    Serializer* valSerializer;
} Btree;

/*
 * Create a btree, returns the pointer.
 * We pass in the key comparison function for future key compares
 */
Btree* btreeCreate(Serializer*, Serializer*, int (*keyCompare) (const void*, const void*));
void insert(Btree* btree, void* key, void* value);
//void* range(Btree* btree, void* lo, void* hi);
void printElements(Btree* btree, void (*printKeyVal) (void*, void*));
void btreeMetadata(Btree* btree);
int btreeFree(Btree* btree);

