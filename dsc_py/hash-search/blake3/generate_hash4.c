//#define _GNU_SOURCE
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "blake3.h"
#define HASH_SIZE 8
// const int HASH_SIZE = 8;
const size_t COUNT = 67108864;
const int nonce_size=8;
//const int numOfBuckets=10;
const int bucket_size=524288;
const int num_threads=1;


typedef struct hashObject
{
    char byteArray[HASH_SIZE];
    long int nonce;
}hashObject;

//char byteArray;
//hashObject *hash_results;

void printArray(unsigned char byteArray[HASH_SIZE], int arraySize)
{
    printf("printArray(): ");
    for (size_t i = 0; i < arraySize; i++)
    {
        printf("%02x ", byteArray[i]); // Print each byte in hexadecimal format
    }
    printf("\n");
}

//void generate_hashes(void *arg)
void generate_hashes(size_t start_index,hashObject h[COUNT])
{
    size_t num_hashes_each_thread=COUNT/num_threads;
    //size_t start_index=*((size_t *)arg);
    size_t end_index=start_index+num_hashes_each_thread-1;

    //hashObject *hash_results_local=(hashObject *)malloc(sizeof(hashObject)*COUNT);
    
    //hashObject *local=malloc(sizeof(hashObject)*num_hashes);

    for (size_t i = start_index; i <= end_index; i++)
    {
        char random_data[HASH_SIZE];
        for (size_t j = 0; j < HASH_SIZE; j++)
        {
            random_data[j] = rand() % 256;
        }

        blake3_hasher hasher;
        blake3_hasher_init(&hasher);
        blake3_hasher_update(&hasher, random_data, HASH_SIZE);
        blake3_hasher_finalize(&hasher, h[i].byteArray, HASH_SIZE);
        h[i].nonce=i;
        // printf("%02x", hash_results[i][j]);
        // if ( (i+1) % 250000000 == 0) {
        //     printf("Created 250M hashed");
        // }
    }
    pthread_exit(NULL);
}

int main()
{
    printf("Hello");
    struct hashObject *hash_results = (struct hashObject *)malloc(COUNT*sizeof(struct hashObject));
    //hash_results=(hashObject *)malloc(sizeof(hashObject)*COUNT);
    printf("Hello World");
    if (hash_results == NULL)
    {
        perror("Error allocating memory");
        return 1;
    }
    srand(time(NULL));
    clock_t start=clock();
    pthread_t thread1;
    //pthread_t thread2;
    // Pass the number of structs in the array and the start and end indexes to each thread.
    int index1=0;
    //int index2=COUNT/num_threads;

    // Create the two threads.
    //pthread_create(&thread1, NULL, generate_hashes, &index1);
    //pthread_create(&thread2, NULL, generate_hashes, &index2);

    // Wait for the two threads to finish executing.
    //pthread_join(thread1, NULL);
    //pthread_join(thread2, NULL);

    generate_hashes(index1,hash_results);

    clock_t end=clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Elapsed time: %lf seconds\n", elapsed_time);

    free(hash_results);
    return 0;
}