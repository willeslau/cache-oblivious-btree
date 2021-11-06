#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#pragma once

#define ensure(expr, msg) if (!expr) { perror(msg); exit(EXIT_FAILURE); }

/**
 * This is only a simple serialization implementation
 */
typedef struct Serializer {
    unsigned int itemSize;
    char* (*process) (void*);
} Serializer;

unsigned int serializeUInt(char* dst, unsigned int src, unsigned int index);
unsigned int serializeLong(char* dst, long src, unsigned int index);
unsigned int serializeBool(char* dst, bool src, unsigned int index);
unsigned int copy(char* dst, const char* src, unsigned int len, unsigned int index);
void write(char filename[], char* data);

// bit operations
unsigned int mostSigBit(unsigned int num);
unsigned int ceilDivision(unsigned int a, unsigned int b);
unsigned int normalize(int num);
unsigned int normalizeUint(unsigned int num);