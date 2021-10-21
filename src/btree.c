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
}

// TODO: implement the split function
void _split(BtreeNode* node, const void* key, const void* value) {

}

int _nodeEmpty(BtreeNode* node) {
    return node->size == 0;
}

void _insertLeaf(BtreeNode* node, int (*keyCompare) (const void*, const void*), void* key, void* value) {
    ensure((node->is_leaf == true), "Not leaf node");

    if (node->size > MAX_CHILDREN) return _split(node, key, value);

    int insertPnt = 0;
    while (insertPnt < node->size && keyCompare(key, node->keys[insertPnt]) > 0)
        insertPnt++;

    for (int i = node->size; i > insertPnt; i--) {
        node->keys[i] = node->keys[i-1];
        node->items[i] = node->items[i-1];
    }

    node->keys[insertPnt] = key;
    node->items[insertPnt] = value;
    node->size++;
}

/* Create an new leaf node
 * TODO: we have invoked 3 malloc calls, can reduce to 1?
 */
BtreeNode* _leafNode() {
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
BtreeNode* _newNode() {
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

void insert(Btree* btree, void* key, void* value) {
    // empty tree, create the leaf nodes
    if (btree->root == NULL) {
        btree->root = _leafNode();
        _insertLeaf(btree->root, btree->keyCompare, key, value);
    }

    btree->size++;
}
