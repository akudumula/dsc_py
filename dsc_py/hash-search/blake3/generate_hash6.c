#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "blake3.h"
#define HASH_SIZE 8


// const int HASH_SIZE = 8;
const int COUNT = 671088640;   //1gb file
//const int COUNT = 268435456;
const int nonce_size=8;
//const int numOfBuckets=10;
const int bucket_size=524288;
const int num_threads=32;

struct hashObject
{
    char byteArray[HASH_SIZE];
    long int nonce;
};

struct hashObject *hashresults;

void printArray(unsigned char byteArray[HASH_SIZE], int arraySize)
{
    printf("printArray(): ");
    for (size_t i = 0; i < arraySize; i++)
    {
        printf("%02x ", byteArray[i]); // Print each byte in hexadecimal format
    }
    printf("\n");
}

void *generate_hashes(void *arg)
{
    int thread_id = *(int *)arg;
    printf("Thread Id - %d\n", thread_id);
    unsigned int seed = time(NULL) + thread_id;
    int start = thread_id * (COUNT / num_threads);
    int end = ((thread_id+1)*(COUNT/num_threads));

    printf("Thread Id - %d, Start - %d\n", thread_id, start);
    printf("Thread Id - %d, End - %d\n", thread_id, end);

    for (int k=start; k < end; k++)
    {
        char random_data[8];
        for (size_t j = 0; j < HASH_SIZE; j++)
        {
            random_data[j] = rand_r(&seed) % 256;
            //random_data[j] = k % 256;
        }

        blake3_hasher hasher;
        blake3_hasher_init(&hasher);
        blake3_hasher_update(&hasher, random_data, HASH_SIZE);
        blake3_hasher_finalize(&hasher, hashresults[k].byteArray, HASH_SIZE);
        hashresults[k].nonce=k;
        // if ( k % 1000000 == 0 ) {
        //     printf("Thread Id - %d, Index - %d\n", thread_id, k);
        // }
    }

    pthread_exit(NULL);
    return NULL;
}


int main()
{
    struct timeval start_time, end_time, time_result;
    hashresults = ( struct hashObject *)malloc(COUNT*sizeof( struct hashObject));    
    if (hashresults == NULL)
    {
        perror("Error allocating memory");
        return 1;
    }

    srand(time(NULL));

    pthread_t threads[num_threads];
    int threadsIds[num_threads];
    gettimeofday(&start_time, NULL);

    for (int i = 0; i < num_threads; i++)
    {
        threadsIds[i] = i;
        pthread_create(&threads[i], NULL, generate_hashes, &threadsIds[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    } 

    gettimeofday(&end_time, NULL);

    timersub(&end_time, &start_time, &time_result);
    printf("Elapsed time: %ld.%06ld\n", (long int) time_result.tv_sec, (long int) time_result.tv_usec);
    printArray(hashresults[0].byteArray,8);
    // printArray(hashresults[67108864].byteArray,8);
    // printArray(hashresults[134217728].byteArray,8);
    // printArray(hashresults[201326592].byteArray,8);
    // printArray(hashresults[268435455].byteArray,8);
    // printArray(hashresults[5].byteArray,8);
    // printArray(hashresults[6].byteArray,8);
    // printArray(hashresults[7].byteArray,8);
    // printArray(hashresults[8].byteArray,8);
    // printArray(hashresults[9].byteArray,8);

    // int file=open("../hashes_6.bin", O_WRONLY | O_CREAT | O_DIRECT, 0666);
    // if (file==-1)
    // {
    //     perror("Error opening file");
    //     return 1;
    // }

    // for (int i=0;i<COUNT;i++)
    // {
    //     ssize_t bytes_written = write(file, &hashresults[i], sizeof(struct hashObject));
    //     if (bytes_written == -1) {
    //         perror("Error writing to file");
    //         close(file);
    //         return 1;
    //     }

    //     if (bytes_written != sizeof(struct hashObject)) {
    //         fprintf(stderr, "Incomplete write operation\n");
    //         close(file);
    //         return 1;
    //     }
    // }
    gettimeofday(&start_time, NULL);

    FILE *file=fopen("../hashes_6.bin","wb");
    if (file==NULL)
    {
        perror("Error opening file");
        return 1;
    }

    
        for (size_t i=0;i<COUNT;i+=bucket_size)
        {
            size_t bytesWritten=fwrite(&hashresults[i],1,(bucket_size*sizeof(struct hashObject)),file);
            //size_t bytesWritten=fwrite(hash_results[i].byteArray,1,sizeof(hash_results[0].byteArray),file);
            //printf("Bytes Written: %zu",bytesWritten);
            //printf("To be Written: %zu",(bucket_size*sizeof(struct hashObject)));
            if (bytesWritten != (bucket_size*sizeof(struct hashObject)))
            {
                perror("Error writing to file");
                fclose(file);
                return 1;
            }
            // total_bytesWritten+=bytesWritten;
        }
        //printf("%d\n",k);
    

    gettimeofday(&end_time, NULL);

    timersub(&end_time, &start_time, &time_result);
    printf("Elapsed time: %ld.%06ld\n", (long int) time_result.tv_sec, (long int) time_result.tv_usec);
    free(hashresults);

    return 0;
}