#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "blake3.h"
#define HASH_SIZE 8
// const int HASH_SIZE = 8;
const int COUNT = 67108864;
const int nonce_size=8;
//const int numOfBuckets=10;
const int bucket_size=524288;

typedef struct hashObject
{
    char byteArray[HASH_SIZE];
    long int nonce;
}hashObject;

void printArray(unsigned char byteArray[HASH_SIZE], int arraySize)
{
    printf("printArray(): ");
    for (size_t i = 0; i < arraySize; i++)
    {
        printf("%02x ", byteArray[i]); // Print each byte in hexadecimal format
    }
    printf("\n");
}



int main()
{
  //  int bucket_size=COUNT/numOfBuckets;
    double total_time = 0;

    FILE *file=fopen("../hashes_4.bin","wb");

    // Open the file for writing
    // int file = open("../hashes_2.bin", O_CREAT | O_WRONLY | O_DIRECT,0666);
    // if (file == -1)
    // {
    //     perror("Error opening file");
    //     return 1;
    // }

    // Seed the random number generator
    srand(time(NULL));

    // start time

    for (int index = 0; index < 1; index++)
    {

        // Allocate memory for hash results
        //uint8_t(*hash_results)[BLAKE3_OUT_LEN] = malloc(COUNT * sizeof(*hash_results));
        struct hashObject *hash_results = (struct hashObject *)malloc(COUNT*sizeof(struct hashObject));
        
        if (hash_results == NULL)
        {
            perror("Error allocating memory");
            return 1;
        }

        clock_t start_time = clock();

        // Generate and hash random data
        for (size_t i = 0; i < COUNT; i++)
        {
            char random_data[HASH_SIZE];
            for (size_t j = 0; j < HASH_SIZE; j++)
            {
                random_data[j] = rand() % 256;
            }

            blake3_hasher hasher;
            blake3_hasher_init(&hasher);
            blake3_hasher_update(&hasher, random_data, HASH_SIZE);
            blake3_hasher_finalize(&hasher, hash_results[i].byteArray, HASH_SIZE);
            hash_results[i].nonce=i;
            // printf("%02x", hash_results[i][j]);
            // if ( (i+1) % 250000000 == 0) {
            //     printf("Created 250M hashed");
            // }

            if ( i % 5000000 == 0 ) {
                printf("INDEX : %zu\n", i);
            }
        }
        // Record the end time
        clock_t end_time = clock();
        // Calculate the elapsed time in seconds
        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        total_time += elapsed_time;

        // Print the results
        printf("Elapsed time: %lf seconds\n", elapsed_time);    

        printArray(hash_results[21].byteArray,8);

        // start_time = clock();

        // Write the hash results to the file
        // for (size_t i = 0; i < num_hashes; i++)
        // {
        //     for (size_t j = 0; j < 12; j++)
        //     {
        //         fprintf(file, "%02x", hash_results[i][j]);
        //         // printf("%02x", hash_results[i][j]);
        //     }
        //     fprintf(file, " ");
        //     fprintf(file, "%010zu", i);
        //     // printf("%010zu", i);
        //     fprintf(file, "\n");
        //     // printf("\n");
        //     // if ( (i+1) % 250000000 == 0) {
        //     //     printf("Wrote 250M hashed");
        //     // }
        // }

        //BULK Write
        start_time = clock();
        ssize_t total_bytesWritten=0;
        // for (size_t i;i<COUNT/bucket_size;i++)
        // {
        //     ssize_t bytesWritten=write(file,&hash_results[i],(bucket_size*sizeof(struct hashObject)));
        //     //size_t bytesWritten=fwrite(hash_results[i].byteArray,1,sizeof(hash_results[0].byteArray),file);
        //     printf("Bytes Written: %zu",bytesWritten);
        //     printf("To be Written: %zu",(bucket_size*sizeof(struct hashObject)));
        //     if (bytesWritten != (bucket_size*sizeof(struct hashObject)))
        //     {
        //         perror("Error writing to file");
        //         close(file);
        //         return 1;
        //     }
        //     total_bytesWritten+=bytesWritten;
        // }
        
        // trying one hash at a time
        // for (size_t i;i<COUNT;i++)
        // {
        //     ssize_t bytesWritten=write(file,&hash_results[i],16);
        //     printf("%ld\n",bytesWritten);
        //     if (bytesWritten != 16) {
        //         // Error writing to file.
        //         printf("Error writing to file.\n");
        //         close(file);
        //         return 1;
        //         }
        // }

        //bulk writing 1gb file 64 times
        for (int k=0;k<10;k++)
        {
            for (size_t i=0;i<COUNT;i+=bucket_size)
            {
                size_t bytesWritten=fwrite(&hash_results[i],1,(bucket_size*sizeof(struct hashObject)),file);
                //size_t bytesWritten=fwrite(hash_results[i].byteArray,1,sizeof(hash_results[0].byteArray),file);
                //printf("Bytes Written: %zu",bytesWritten);
                //printf("To be Written: %zu",(bucket_size*sizeof(struct hashObject)));
                if (bytesWritten != (bucket_size*sizeof(struct hashObject)))
                {
                    perror("Error writing to file");
                    fclose(file);
                    return 1;
                }
                total_bytesWritten+=bytesWritten;
            }
            printf("%d\n",k);
            end_time = clock();
            // Calculate the elapsed time in seconds
            elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            printf("Elapsed time: %lf seconds\n", elapsed_time);
        }

        //flush(file);

        // Record the end time
        end_time = clock();
        // Calculate the elapsed time in seconds
        elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        total_time += elapsed_time;

        // Print the results
        printf("Elapsed time: %lf seconds\n", elapsed_time);
        printf("%zu",total_bytesWritten);

        //Free allocated memory
        free(hash_results);
    }

    printf("Total time: %lf seconds\n", total_time);

    // Close the file
    fclose(file);

    printf("Hashes written to hashes.bin\n");

    return 0;
}
