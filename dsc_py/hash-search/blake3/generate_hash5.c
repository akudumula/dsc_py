// #define _GNU_SOURCE
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
const int COUNT = 67108864;
const int nonce_size=8;
//const int numOfBuckets=10;
const int bucket_size=524288;
// const int num_threads=4;

struct hashObject
{
    char byteArray[HASH_SIZE];
    long int nonce;
};

struct arraySubsection
{
    struct hashObject *index;
    long int start;
    long int end;
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


void *generate_hashes(void *arg)
{
    struct arraySubsection * hashWork=(struct arraySubsection*)arg;
    size_t start=hashWork->start;
    size_t end=hashWork->end;
    printArray(hashWork->index->byteArray,8);
    printf("Start: %zu\n", start);
    printf("End: %zu\n", end);
    for (size_t k=start;k<=end;k++)
    {
        char random_data[8];
        for (size_t j = 0; j < 8; j++)
        {
            //random_data[j] = rand() % 256;
            random_data[j] = "a";
        }

        blake3_hasher hasher;
        blake3_hasher_init(&hasher);
        blake3_hasher_update(&hasher, random_data, 8);
        blake3_hasher_finalize(&hasher, hashWork->index[k].byteArray, 8);
        hashWork->index[k].nonce=k;
        // printf("%02x", hash_results[i][j]);
        // if ( (i+1) % 250000000 == 0) {
        //     printf("Created 250M hashed");
        // }
    }
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    //hashObject hashresults[COUNT];
    struct timeval start_time, end_time, time_result;
    struct hashObject *hashresults = ( struct hashObject *)malloc(COUNT*sizeof( struct hashObject));    
    if (hashresults == NULL)
    {
        perror("Error allocating memory");
        return 1;
    }
    int threadCount = 1;
    pthread_t threads[threadCount];
    size_t c=0;
    struct arraySubsection * a=(struct arraySubsection *)malloc(threadCount*sizeof(struct arraySubsection));
    if (a== NULL)
    {
        perror("Error allocating memory to arraySubsection");
        return 1;
    }
    hashresults[0]=(struct hashObject){.byteArray="abcdefgh",.nonce=0};
    hashresults[(COUNT/threadCount)]=(struct hashObject){.byteArray="abcdefgh",.nonce=0};
    for (int i=0;i<threadCount;i++)
    {
        a[i].index=&hashresults[i*(COUNT/threadCount)];
        a[i].start=i*(COUNT/threadCount);
        a[i].end=((i+1)*(COUNT/threadCount)-1);
    }
    for (int i=0;i<threadCount;i++)
    {
        printArray(a->index->byteArray,8);
        printf("Start: %zu for index %d\n",a[i].start,i);
        printf("End: %zu for index %d\n",a[i].end,i);   
    }
    
    srand(time(NULL));
    //clock_t start_time = clock();
    gettimeofday(&start_time, NULL);
    // printf("%d",(3*(COUNT/num_threads)));
    for (int i = 0; i < threadCount; i++) 
    {
        // struct arraySubsection * a=(struct arraySubsection *)malloc(sizeof(struct arraySubsection));
        pthread_create(&threads[i], NULL, generate_hashes, &(a[i]));
    }
    for (int i = 0; i < threadCount; i++) 
    {
        pthread_join(threads[i], NULL);
    }
    
    // clock_t end_time = clock();
    // Calculate the elapsed time in seconds
    // double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    gettimeofday(&end_time, NULL);

    timersub(&end_time, &start_time, &time_result);
    printf("Elapsed time: %ld.%06ld\n", (long int) time_result.tv_sec, (long int) time_result.tv_usec);

    // long long elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000000LL + (end_time.tv_usec - start_time.tv_usec);
    printArray(hashresults[21].byteArray,8);
    // double generation_time = (double)elapsed / 1000000.0;
    // printf("Elapsed time: %lf seconds\n", generation_time);

    FILE *file=fopen("../hashes_5.bin","wb");

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
        //total_bytesWritten+=bytesWritten;
    }

    free(hashresults);

    return 0;
}