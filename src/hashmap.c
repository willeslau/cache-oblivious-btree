//
// Created by lxm on 26/10/21.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree.h"

#define SIZE 10000

struct DataItem* dummyItem;

int hashCode(int key) {
    return key % SIZE;
}

struct DataItem* search(DataItem** hashArray, int key) {
    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while(hashArray[hashIndex] != NULL) {

        if(hashArray[hashIndex]->key == key)
            return hashArray[hashIndex];

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

void insertHashMap(DataItem** hashArray, int key, void* data) {
    struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
    item->data = data;
    item->key = key;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty or deleted cell
    while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    hashArray[hashIndex] = item;
}

struct DataItem* delete(DataItem** hashArray, DataItem* item) {
    int key = item->key;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while(hashArray[hashIndex] != NULL) {

        if(hashArray[hashIndex]->key == key) {
            struct DataItem* temp = hashArray[hashIndex];

            //assign a dummy item at deleted position
            hashArray[hashIndex] = dummyItem;
            return temp;
        }

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

//void display() {
//    int i = 0;
//
//    for(i = 0; i<SIZE; i++) {
//
//        if(hashArray[i] != NULL)
//            printf(" (%d,%d)",hashArray[i]->key,hashArray[i]->data->id);
//        else
//            printf(" ~~ ");
//    }
//
//    printf("\n");
//}
