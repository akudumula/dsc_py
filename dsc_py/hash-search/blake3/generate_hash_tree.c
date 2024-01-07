#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "blake3.h"
#define HASH_SIZE 8
#define MAX_CHILDREN 256

const int BLOCK_SIZE =131072;

// const int HASH_SIZE = 8;
//const int COUNT = 67108864;   //1gb file
const int COUNT = 2097152;
//const int COUNT = 268435456;
const int nonce_size=8;
//const int numOfBuckets=10;
//const int block_size=524288;
const int num_threads=16;

struct hashObject
{
    char byteArray[HASH_SIZE];
};


struct leaf
{
    int size_of_block_index;
    int* block_index;
    struct hashObject* hash_block;
    int hash_block_index;
};

struct child1
{
    struct leaf* children[MAX_CHILDREN];
};

struct root
{
    struct child1* children[MAX_CHILDREN];
};

struct hashObject *hashflushblock;
struct root *hashtree;


// void printArray(unsigned char byteArray[HASH_SIZE], int arraySize)
// {
//     printf("printArray(): ");
//     for (size_t i = 0; i < arraySize; i++)
//     {
//         printf("%02x ", byteArray[i]); // Print each byte in hexadecimal format
//     }
//     printf("\n");
// }




int main()
{

    FILE *file=fopen("../hashetree_1.bin","wb");

    struct timeval start_time, end_time, time_result;
    printf("Creating hash tree structure\n");
    gettimeofday(&start_time, NULL);
    hashtree = (struct root *)malloc(sizeof(struct root));
    if (hashtree == NULL)
    {
        perror("Error allocating memory : root");
        return 1;
    }
    //printf("Creating child \n");
    for (int i =0;i<MAX_CHILDREN;i++)
    {
        hashtree->children[i]=(struct child1 *)malloc(sizeof(struct child1));
        if (hashtree->children[i] == NULL)
        {
            perror("Error allocating memory : child 1");
            return 1;
        }
        //printf("Creating leaf\n");
        for (int j=0;j<256;j++)
        {
            hashtree->children[i]->children[j]=(struct leaf *)malloc(sizeof(struct leaf));
            if (hashtree->children[i]->children[j] == NULL)
            {
                perror("Error allocating memory: child 2");
                return 1;
            }
            //printf("Creating hash block\n");
            hashtree->children[i]->children[j]->block_index=malloc(1*(sizeof(int)));
            hashtree->children[i]->children[j]->hash_block=(struct hashObject*)malloc(BLOCK_SIZE * sizeof(struct hashObject));
            if (hashtree->children[i]->children[j]->hash_block == NULL)
            {
                perror("Error allocating memory: hash_block");
                return 1;
            }
            hashtree->children[i]->children[j]->hash_block_index=0;
            hashtree->children[i]->children[j]->size_of_block_index=1;
            
        }
    }

    printf("Value at array: %ld\n",sizeof(hashtree->children[0]->children[0]->block_index));

    

    // hashresults = ( struct hashObject *)malloc(COUNT*sizeof( struct hashObject));    
    // if (hashresults == NULL)
    // {
    //     perror("Error allocating memory");
    //     return 1;
    // }
    gettimeofday(&end_time, NULL);

    timersub(&end_time, &start_time, &time_result);
    printf("Elapsed time: %ld.%06ld\n", (long int) time_result.tv_sec, (long int) time_result.tv_usec);
    printf("No errors?\n");
    srand(time(NULL));

    gettimeofday(&start_time,NULL);
    printf("Generating Hashes\n");
    hashflushblock = ( struct hashObject *)malloc(BLOCK_SIZE*4*sizeof( struct hashObject));  

    //char c=getchar();
    
    //Generating Hashes

    size_t block_number=0;
    for (int k=0; k < COUNT; k++)
    {
        char random_data[8];
        for (size_t j = 0; j < HASH_SIZE; j++)
        {
            random_data[j] = rand() % 256;
            //random_data[j] = k % 256;
        }
        unsigned char byteArray[HASH_SIZE];
        //struct hashObject *hash=(struct hashObject*)malloc(sizeof(struct hashObject));
        // char byteArray[HASH_SIZE];
        blake3_hasher hasher;
        blake3_hasher_init(&hasher);
        blake3_hasher_update(&hasher, random_data, HASH_SIZE);
        blake3_hasher_finalize(&hasher,byteArray, HASH_SIZE);
        if (k%100000==0)
        {
            printf("Hashes generated %d\n",k);
        }
        unsigned char bit1 = byteArray[0];
        unsigned char bit2 = byteArray[1];
        // // unsigned char bit3 = byteArray[2];
        int c=hashtree->children[bit1]->children[bit2]->hash_block_index;
        //hashtree->children[bit1]->children[bit2]->children[bit3]->hash_block[c].byteArray=byteArray;
        memcpy(hashtree->children[bit1]->children[bit2]->hash_block[c].byteArray,byteArray,sizeof(struct hashObject));
        
        hashtree->children[bit1]->children[bit2]->hash_block_index+=1;
        if (c==BLOCK_SIZE-1)
        {
            // printf("Block Written = %c %c",bit1,bit2);
            printf("Block Size: %d\n",BLOCK_SIZE);
            printf( "Writing to file - bit1 - %d, bit2 - %d, count - %d\n", bit1, bit2, c );
            size_t bytesWritten=fwrite(&hashtree->children[bit1]->children[bit2]->hash_block[0],1,(BLOCK_SIZE*sizeof(struct hashObject)),file);
            if (bytesWritten != (BLOCK_SIZE*sizeof(struct hashObject)))
            {
                perror("Error writing to file: while generating\n");
                fclose(file);
                return 1;
            }
            free(hashtree->children[bit1]->children[bit2]->hash_block);
            hashtree->children[bit1]->children[bit2]->hash_block_index=0;
            hashtree->children[bit1]->children[bit2]->hash_block=(struct hashObject*)malloc(BLOCK_SIZE * sizeof(struct hashObject));
            if (hashtree->children[bit1]->children[bit2]->hash_block == NULL)
            {
                perror("Error allocating memory: hash_block : Generating Hashes\n");
                return 1;
            }
            hashtree->children[bit1]->children[bit2]->block_index=malloc((hashtree->children[bit1]->children[bit2]->size_of_block_index+1)*(sizeof(int)));
            hashtree->children[bit1]->children[bit2]->block_index[(hashtree->children[bit1]->children[bit2]->size_of_block_index)-1]=block_number;
            hashtree->children[bit1]->children[bit2]->size_of_block_index+=1;
            block_number+=1;
        }

    }

    // for (int i=0;i<256;i++)
    // {
    //     for (int j =0;j<256;j++)
    //     {
    //         if (hashtree->children[i]->children[j]->hash_block_index!=0)
    //         {
    //             size_t bytesWritten=fwrite(&hashtree->children[i]->children[j]->hash_block[0],1,(BLOCK_SIZE*sizeof(struct hashObject)),file);
    //             if (bytesWritten != (BLOCK_SIZE*sizeof(struct hashObject)))
    //             {
    //                 perror("Error writing to file when dumping\n");
    //                 fclose(file);
    //                 return 1;
    //             }
    //             free(hashtree->children[i]->children[j]->hash_block);
    //             hashtree->children[i]->children[j]->hash_block_index=0;
    //             hashtree->children[i]->children[j]->block_index=malloc((hashtree->children[i]->children[j]->size_of_block_index+1)*(sizeof(int)));
    //             hashtree->children[i]->children[j]->block_index[(hashtree->children[i]->children[j]->size_of_block_index)-1]=block_number;
    //             hashtree->children[i]->children[j]->size_of_block_index+=1;
    //             block_number+=1;
    //         }
    //     }
    // }

    gettimeofday(&end_time, NULL);

    timersub(&end_time, &start_time, &time_result);
    printf("Elapsed time: %ld.%06ld\n", (long int) time_result.tv_sec, (long int) time_result.tv_usec);
    printf("No errors?\n");
    


    //     // if ( k % 1000000 == 0 ) {
    //     //     printf("Thread Id - %d, Index - %d\n", thread_id, k);
    //     // }
    // }


    // pthread_t threads[num_threads];
    // int threadsIds[num_threads];
    // gettimeofday(&start_time, NULL);

    // for (int i = 0; i < num_threads; i++)
    // {
    //     threadsIds[i] = i;
    //     pthread_create(&threads[i], NULL, generate_hashes, &threadsIds[i]);
    // }

    // for (int i = 0; i < num_threads; i++) {
    //     pthread_join(threads[i], NULL);
    // } 

    // gettimeofday(&end_time, NULL);

    // timersub(&end_time, &start_time, &time_result);
    // printf("Elapsed time: %ld.%06ld\n", (long int) time_result.tv_sec, (long int) time_result.tv_usec);
    // printArray(hashresults[0].byteArray,8);
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
    // gettimeofday(&start_time, NULL);

    // FILE *file=fopen("../hashes_6.bin","wb");
    // if (file==NULL)
    // {
    //     perror("Error opening file");
    //     return 1;
    // }

    // for (int k=0;k<10;k++)
    // {
    //     for (size_t i=0;i<COUNT;i+=bucket_size)
    //     {
    //         size_t bytesWritten=fwrite(&hashresults[i],1,(bucket_size*sizeof(struct hashObject)),file);
    //         //size_t bytesWritten=fwrite(hash_results[i].byteArray,1,sizeof(hash_results[0].byteArray),file);
    //         //printf("Bytes Written: %zu",bytesWritten);
    //         //printf("To be Written: %zu",(bucket_size*sizeof(struct hashObject)));
    //         if (bytesWritten != (bucket_size*sizeof(struct hashObject)))
    //         {
    //             perror("Error writing to file");
    //             fclose(file);
    //             return 1;
    //         }
    //         // total_bytesWritten+=bytesWritten;
    //     }
    //     printf("%d\n",k);
    // }

    

    // gettimeofday(&end_time, NULL);

    // timersub(&end_time, &start_time, &time_result);
    // printf("Elapsed time: %ld.%06ld\n", (long int) time_result.tv_sec, (long int) time_result.tv_usec);
    free(hashtree);
    free(hashflushblock);

    return 0;
}