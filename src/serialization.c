#include <stdbool.h>
#include <stdio.h>

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
