/*
 * The cache oblivious btree implementation
 */
#include "util.h"

typedef struct CacheObliviousBTree {

} COB;

COB* btreeCreate(Serializer*, Serializer*, int (*keyCompare) (const void*, const void*));
void insert(COB* cob, void* key, void* value);