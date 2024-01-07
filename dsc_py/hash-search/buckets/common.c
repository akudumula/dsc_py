#include <stdlib.h>
#include <math.h>

#define HASH_SIZE 9
#define PREFIX_SIZE 1

struct hashObject
{
    char byteArray[HASH_SIZE - PREFIX_SIZE];
    long int value;
};

void printArray(unsigned char byteArray[HASH_SIZE], int arraySize)
{
    printf("printArray(): ");
    for (size_t i = 0; i < arraySize; i++)
    {
        printf("%02x ", byteArray[i]); // Print each byte in hexadecimal format
    }
    printf("\n");
}

int compareHashObjects(const void *a, const void *b)
{
    const struct hashObject *hashObjA = (const struct hashObject *)a;
    const struct hashObject *hashObjB = (const struct hashObject *)b;

    // Compare the byteArray fields
    return memcmp(hashObjA->byteArray, hashObjB->byteArray, sizeof(hashObjA->byteArray));
}

const int SEARCH_COUNT = 1000;
const int hashesPerBucket = 256 * 1024; 
const size_t hashesPerBucketRead = 256 * 256 * 256;
const size_t sortSize = 16; //In MB
const size_t maxHashesToSort = (sortSize * 1024 * 1024) / sizeof(struct hashObject);
const size_t numberOfHashesInBucket = hashesPerBucketRead < maxHashesToSort ? hashesPerBucketRead : maxHashesToSort ; 
const int NUM_THREADS = 2;   