#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "util.h"

unsigned int serializeUInt(char* dst, unsigned int src, unsigned int index) {
    for (int i = 0; i < 4; i++) {
        dst[index] = (src % 10) + '0';
        src /= 10;
        index++;
    }
    return index;
}

unsigned int serializeLong(char* dst, long src, unsigned int index) {
    for (int i = 0; i < 8; i++) {
        dst[index] = (src % 10) + '0';
        src /= 10;
        index++;
    }
    return index;
}

unsigned int serializeBool(char* dst, bool src, unsigned int index) {
    dst[index] = src == true ? '1' : '0';
    return index+1;
}

unsigned int copy(char* dst, const char* src, unsigned int len, unsigned int index) {
    for (unsigned int i = 0; i < len; i++) {
        dst[index] = src[i];
        index++;
    }
    return index;
}

void write(char filename[], char* data) {
    FILE *file = fopen(filename, "w");

    int results = fputs(data, file);
    if (results == EOF) {
        // Failed to write do error code here.
    }
    fclose(file);
}

unsigned int normalize(int num) {
    unsigned int i = 1;
    while (i < num) {
        i  = i << 1;
    }
    return i;
}

/*
 * Returns the 1-based index of the last (i.e., most significant) bit set in x.
 */
unsigned int lastBitSet (unsigned int x) {
    ensure((x > 0), "invalid input");
    return (sizeof(unsigned int) * 8 - __builtin_clz(x));
}

unsigned int floorLg (unsigned int x) {
    return (lastBitSet (x) - 1);
}

unsigned int ceilLg (unsigned int x) {
    return (lastBitSet (x - 1));
}

/* Returns the largest power of 2 not greater than x
 * (i.e., $2^{\lfloor \lg x \rfloor}$).
 */
unsigned int hyperFloor (unsigned int x) {
    return (1 << floorLg (x));
}

/* Returns the smallest power of 2 not less than x
 * (i.e., $2^{\lceil \lg x \rceil}$).
 */
unsigned int hyperCeil (unsigned int x) {
    return (1 << ceilLg (x));
}

unsigned int ceilDiv (unsigned int x, unsigned int y) {
    ensure((x > 0), "invalid input x");
    return (1 + ((x - 1) / y));
}
