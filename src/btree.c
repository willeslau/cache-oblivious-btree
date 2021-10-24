/*
 * The implementation of Btree functions
 */

#include <malloc.h>
#include <stdlib.h>
#include "btree.h"

Btree* btreeCreate(int (*keyCompare) (const void* key1, const void* key2)) {
    Btree* btree = malloc(sizeof(*btree));
    if (btree == NULL) return NULL;

    btree->height = 0;
    btree->size = 0;
    btree->keyCompare = keyCompare;
    btree->root = NULL;
    return btree;
}

/* Create an new leaf node
 * TODO: we have invoked 3 malloc calls, can reduce to 1?
 */
BtreeNode* leafNode() {
    BtreeNode* node = malloc(sizeof(*node));
    ensure((node != NULL), "Cannot init node pointer.");

    node->size = 0;
    node->parent = NULL;
    node->links = NULL;

    int capacity = MAX_CHILDREN;
    node->keys = malloc(capacity * sizeof(void*));
    ensure((node->keys != NULL), "Cannot init node keys pointers array.");

    node->is_leaf = true;
    node->items = malloc(capacity * sizeof(void*));
    ensure((node->items != NULL), "Cannot init node items pointers array.");

    node->next = NULL;
    node->prev = NULL;

    return node;
}

/* Create an new empty node
 * TODO: we have invoked 3 malloc calls, can reduce to 1?
 */
BtreeNode* linkNode() {
    BtreeNode* node = malloc(sizeof(*node));
    ensure((node != NULL), "Cannot init node pointer.");

    node->size = 0;
    node->parent = NULL;

    node->keys = malloc(MAX_CHILDREN * sizeof(void*));
    ensure((node->keys != NULL), "Cannot init node keys pointers array.");

    node->links = malloc((1 + MAX_CHILDREN) * sizeof(void*));
    ensure((node->links != NULL), "Cannot init node links pointers array.");

    node->is_leaf = false;
    node->items = NULL;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

int nodeFull(BtreeNode* node) {
    return node->size == MAX_CHILDREN;
}

/* Split the node `toSplit` into two nodes. The hiNode holds keys
 * above the median. After the split, toSplit holds the keys below
 * the median key.
 * The parentNode is the parent of toSplit. The index i is where in
 * the parent the newly splitted nodes should be linked to.
 * Leaf and link nodes should be handled differently.
 * Steps are as follows:
 *  1. Init node
 *  2. Add the newly added key along with toSplit keys,
 *     find the median of the combined keys.
 */
void nodeSplit(Btree* btree, BtreeNode* parentNode, int i, BtreeNode* toSplit, void* key) {
    // init the hiNode
    BtreeNode* hiNode;
    if (toSplit->is_leaf) hiNode = leafNode();
    else hiNode = linkNode();

    // now copy the links of `toSplit` to `hiNode`
    int median = MAX_CHILDREN / 2;
    if (toSplit->is_leaf) {
        int k = 0;
        for (int j = median; j < MAX_CHILDREN; j++) {
            hiNode->items[k] = toSplit->items[j];
            hiNode->keys[k] = toSplit->keys[j];
            k++;
        }
        hiNode->next = toSplit->next;
        hiNode->prev = toSplit;
        toSplit->next = hiNode;
    } else {
        int j;
        int k = 1;
        // links[median] belongs to `toSplit` node
        hiNode->keys[0] = toSplit->keys[median];
        for (j = median+1; j <= MAX_CHILDREN - 1; j++) {
            hiNode->links[k] = toSplit->links[j];
            hiNode->keys[k] = toSplit->keys[j];
            k++;
        }
        hiNode->links[k] = toSplit->links[j];
        toSplit->next = NULL;
        toSplit->prev = NULL;
    }
    hiNode->size = MAX_CHILDREN - median;
    toSplit->size -= hiNode->size;

    // now copy the links to the parent
    for (int j = parentNode->size-1; j >= i; j--) {
        // index will not overflow as parentNode is not full
        parentNode->keys[i+1] = parentNode->keys[i];
        parentNode->links[i+2] = parentNode->links[i+1];
    }

    // insert to median key of toSplit into the parent
    parentNode->keys[i] = toSplit->keys[median];
    parentNode->links[i+1] = hiNode;
    parentNode->links[i] = toSplit;
    parentNode->size++;

    hiNode->parent = parentNode;
    toSplit->parent = parentNode;
}

/* Insert into a leaf node. Caller ensure the node is a leaf node. */
void insertLeaf(Btree* btree, BtreeNode* node, int insertPnt, void* key, void* value) {
    for (int i = node->size; i > insertPnt; i--) {
        node->keys[i] = node->keys[i-1];
        node->items[i] = node->items[i-1];
    }
    node->keys[insertPnt] = key;
    node->items[insertPnt] = value;
    node->size++;
    btree->size++;
}

void nodeInsertNonFull(Btree* btree, BtreeNode* node, void* key, void* value) {
    // TODO: count down will be slightly faster
    int insertPnt = 0;
    while (insertPnt < node->size && btree->keyCompare(key, node->keys[insertPnt]) > 0)
        insertPnt++;

    if (node->is_leaf) {
        if (node->keys[insertPnt] == NULL || btree->keyCompare(key, node->keys[insertPnt]) != 0) insertLeaf(btree, node, insertPnt, key, value);
        else node->items[insertPnt] = value;
    } else {
        if (nodeFull(node->links[insertPnt])) {
            nodeSplit(btree, node, insertPnt, node->links[insertPnt], key);
            if (btree->keyCompare(node->keys[insertPnt], key) < 0) {
                insertPnt++;
            }
        }
        nodeInsertNonFull(btree, node->links[insertPnt], key, value);
    }
}

void insert(Btree* btree, void* key, void* value) {
    // empty tree, create the leaf nodes
    if (btree->root == NULL) {
        btree->root = leafNode();
        btree->height++;
    }

    if (nodeFull(btree->root)) {
        BtreeNode* node = btree->root;
        btree->root = linkNode();
        node->parent = btree->root;
        nodeSplit(btree, btree->root, 0, node, key);
        btree->height++;
    }
    nodeInsertNonFull(btree, btree->root, key, value);
}

void printElements(Btree* btree, void (*printKeyVal) (void*, void*)) {
    BtreeNode* node = btree->root;
    while (!node->is_leaf) node = node->links[0];
    while (node != NULL) {
        for (int i = 0; i < node->size; i++)
            printKeyVal(node->keys[i], node->items[i]);
        node = node->next;
    }
    printKeyVal(NULL, NULL);
}

void btreeMetadata(Btree* btree) {
    printf("tree height: %d. ", btree->height);
    printf("tree size: %d\n", btree->size);
}