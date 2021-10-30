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