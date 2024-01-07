#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

const int HASH_SIZE = 9;
const int PREFIX_SIZE = 1; // 3B
const int COUNT = 100;
const int BUCKET_SIZE = 10;

struct hashObject
{
    char byteArray[HASH_SIZE];
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

    FILE *file1 = fopen("test.memo", "wb"); // Open for appending
    if (file1 == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    FILE *file2 = fopen("test.txt", "w"); // Open for appending
    if (file2 == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    int NUM_BUCKETS = 5;

    long long desiredFileSize = 2 * NUM_BUCKETS * BUCKET_SIZE * sizeof(struct hashObject);
    if (fseeko(file1, desiredFileSize - 1, SEEK_SET) != 0)
    {
        perror("Error seeking in file");
        fclose(file1);
        return 1;
    }
    // Write a single byte at the desired position to set the file size
    fputc(0, file1);

    struct hashObject **array = (struct hashObject **) malloc( sizeof(struct hashObject *) * NUM_BUCKETS );
    unsigned char **search_array = (unsigned char **)malloc(BUCKET_SIZE * NUM_BUCKETS * sizeof(char *));
    
    for (int i = 0; i < NUM_BUCKETS; i++)
    {
        array[i] = (struct hashObject *) malloc( sizeof(struct hashObject) * BUCKET_SIZE );
        for(int j = 0; j < BUCKET_SIZE; j++) {
            // array[i][j] = (struct hashObject *) malloc( sizeof(struct hashObject) );
            search_array[i*BUCKET_SIZE+j] = (unsigned char *)malloc( sizeof(unsigned char) * HASH_SIZE);
        }
    }

    int NONCE = 0;
    unsigned char randomArray[HASH_SIZE];

    for (size_t i = 0; i < NUM_BUCKETS; i++)
    {

        if (fseeko(file1, ((i*2)+1)*BUCKET_SIZE*sizeof(struct hashObject), SEEK_SET) != 0)
        {
            perror("Error seeking in file");
            fclose(file1);
            fclose(file2);
            return 1;
        }

        for (size_t j = 0; j < BUCKET_SIZE; j++)
        {
            generateRandomByteArray(randomArray);
            memcpy(array[i][j].byteArray, randomArray, sizeof(randomArray) );
            memcpy(search_array[i*BUCKET_SIZE+j], randomArray, HASH_SIZE );
            array[i][j].value = NONCE;

            // size_t bytesWritten = fwrite(array[i][j], 1, sizeof(struct hashObject), file1);
            // if (bytesWritten != sizeof(struct hashObject))
            // {
            //     perror("Error writing to file");
            //     fclose(file1);
            //     fclose(file2);
            //     return 1;
            // }

            // printArray(randomArray, HASH_SIZE);
            // printArray(array[i]->byteArray, HASH_SIZE);
            // printArray(search_array[i], HASH_SIZE);
            NONCE++;
        }


        size_t bytesWritten = fwrite(array[i], 1, BUCKET_SIZE*sizeof(struct hashObject), file1);
        if (bytesWritten != (BUCKET_SIZE * sizeof(struct hashObject)))
        {
            perror("Error writing to file");
            fclose(file1);
            fclose(file2);
            return 1;
        }
    }
    
    fclose(file1);

    // Write the hash results to the file
    for (size_t k = 0; k < (NUM_BUCKETS * BUCKET_SIZE); k++) {
        for (size_t j = 0; j < HASH_SIZE; j++) {
            fprintf(file2, "%02x", search_array[k][j]);
        }
        fprintf(file2, "\n");
    }
    //Close file
    fclose(file2);


    for (int i = 0; i < NUM_BUCKETS; i++)
    {
        for(int j = 0; j < BUCKET_SIZE; j++) {
            free(search_array[i*BUCKET_SIZE + j]);
        }
        free(array[i]);
    }
    free(array);
    free(search_array);


}