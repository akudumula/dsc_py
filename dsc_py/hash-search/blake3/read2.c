#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

const int HASH_SIZE = 9;
const int PREFIX_SIZE = 1; // 3B
const int COUNT = 100;
const int BUCKET_SIZE = 256*256;

struct hashObject
{
    char byteArray[HASH_SIZE-PREFIX_SIZE];
    long int value;
};

void generateRandomByteArray(unsigned char result[HASH_SIZE])
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        result[i] = rand() % 256; // Generate a random byte (0-255)
    }
}

void printArray(unsigned char byteArray[HASH_SIZE], int arraySize)
{
    printf("printArray(): ");
    for (size_t i = 0; i < arraySize; i++)
    {
        printf("%02x ", byteArray[i]); // Print each byte in hexadecimal format
    }
    printf("\n");
}


int main() {

    FILE *file1 = fopen("plot.memo", "rb"); // Open for appending
    if (file1 == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    int numBuckets = (int)pow(2, PREFIX_SIZE * 8);
    int hashesPerBucketRead = 256 * 256;
    long int MAX_HASHES = 4 * 256;
    int NUM_BUCKETS = (int)pow(2, PREFIX_SIZE * 8);


    struct hashObject *array = (struct hashObject *) malloc( sizeof(struct hashObject) * BUCKET_SIZE );
    // for (size_t i = 0; i < MAX_HASHES; i++)
    // {
    //     array[i] = (struct hashObject *) malloc( sizeof(struct hashObject) );
    // }

    for (size_t i = 0; i < NUM_BUCKETS; i++)
    {
        size_t bytesRead = fread(array, 1, BUCKET_SIZE*sizeof(struct hashObject), file1);
        if (bytesRead !=  BUCKET_SIZE*sizeof(struct hashObject))
        {
            perror("Error reading file");
            fclose(file1);
            return 1;
        }

        for (size_t j = 0; j < BUCKET_SIZE; j++)
        {
            if ( array[j].byteArray[0] > 0 ) {
                printArray(array[j].byteArray, HASH_SIZE-PREFIX_SIZE);
            }
        }
    }


    fclose(file1);
    // for (int i = 0; i < COUNT; i++)
    // {
    //     free(array[i]);
    // }
    free(array);

    return 0;
}